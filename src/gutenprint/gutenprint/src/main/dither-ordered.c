/*
 * "$Id: dither-ordered.c,v 1.19 2007/05/27 18:38:31 rlk Exp $"
 *
 *   Ordered dither algorithm
 *
 *   Copyright 1997-2003 Michael Sweet (mike@easysw.com) and
 *	Robert Krawitz (rlk@alum.mit.edu)
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Revision History:
 *
 *   See ChangeLog
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <gutenprint/gutenprint.h>
#include "gutenprint-internal.h"
#include <gutenprint/gutenprint-intl-internal.h>
#include "dither-impl.h"
#include "dither-inlined-functions.h"

static inline void
print_color_ordered(const stpi_dither_t *d, stpi_dither_channel_t *dc, int val,
		    int x, int y, unsigned char bit, int length)
{
  int i;
  int j;
  unsigned bits;
  int levels = dc->nlevels - 1;

  /*
   * Look for the appropriate range into which the input value falls.
   * Notice that we use the input, not the error, to decide what dot type
   * to print (if any).  We actually use the "density" input to permit
   * the caller to use something other that simply the input value, if it's
   * desired to use some function of overall density, rather than just
   * this color's input, for this purpose.
   */
  for (i = levels; i >= 0; i--)
    {
      stpi_dither_segment_t *dd = &(dc->ranges[i]);

      if (val > dd->lower->value)
	{
	  /*
	   * Where are we within the range.
	   */

	  unsigned rangepoint = val - dd->lower->value;
	  if (dd->value_span < 65535)
	    rangepoint = rangepoint * 65535 / dd->value_span;

	  if (rangepoint >= ditherpoint(d, &(dc->dithermat), x))
	    bits = dd->upper->bits;
	  else
	    bits = dd->lower->bits;

	  if (bits)
	    {
	      unsigned char *tptr = dc->ptr + d->ptr_offset;

	      /*
	       * Lay down all of the bits in the pixel.
	       */
	      set_row_ends(dc, x);
	      for (j = 1; j <= bits; j += j, tptr += length)
		{
		  if (j & bits)
		    tptr[0] |= bit;
		}

	    }
	  return;
	}
    }
}


void
stpi_dither_ordered(stp_vars_t *v,
		    int row,
		    const unsigned short *raw,
		    int duplicate_line,
		    int zero_mask,
		    const unsigned char *mask)
{
  stpi_dither_t *d = (stpi_dither_t *) stp_get_component_data(v, "Dither");
  int		x,
		length;
  unsigned char	bit;
  int i;
  int one_bit_only = 1;

  int xerror, xstep, xmod;

  if ((zero_mask & ((1 << CHANNEL_COUNT(d)) - 1)) ==
      ((1 << CHANNEL_COUNT(d)) - 1))
    return;

  length = (d->dst_width + 7) / 8;

  bit = 128;
  xstep  = CHANNEL_COUNT(d) * (d->src_width / d->dst_width);
  xmod   = d->src_width % d->dst_width;
  xerror = 0;

  for (i = 0; i < CHANNEL_COUNT(d); i++)
    {
      stpi_dither_channel_t *dc = &(CHANNEL(d, i));
      if (dc->nlevels != 1 || dc->ranges[0].upper->bits != 1)
	one_bit_only = 0;
    }

  if (one_bit_only)
    {
      for (x = 0; x < d->dst_width; x ++)
	{
	  if (!mask || (*(mask + d->ptr_offset) & bit))
	    {
	      for (i = 0; i < CHANNEL_COUNT(d); i++)
		{
		  if (raw[i] &&
		      raw[i] >= ditherpoint(d, &(CHANNEL(d, i).dithermat), x))
		    {
		      set_row_ends(&(CHANNEL(d, i)), x);
		      CHANNEL(d, i).ptr[d->ptr_offset] |= bit;
		    }
		}
	    }
	  ADVANCE_UNIDIRECTIONAL(d, bit, raw, CHANNEL_COUNT(d),
				 xerror, xstep, xmod);
	}
    }
  else
    {
      for (x = 0; x != d->dst_width; x ++)
	{
	  if (!mask || (*(mask + d->ptr_offset) & bit))
	    {
	      for (i = 0; i < CHANNEL_COUNT(d); i++)
		{
		  if (CHANNEL(d, i).ptr && raw[i])
		    print_color_ordered(d, &(CHANNEL(d, i)), raw[i], x, row,
					bit, length);
		}
	    }
	  ADVANCE_UNIDIRECTIONAL(d, bit, raw, CHANNEL_COUNT(d), xerror,
				 xstep, xmod);
	}
    }
}
