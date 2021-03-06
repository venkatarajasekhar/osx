/*
 * Copyright (c) 2006 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef __BOOTPLOOKUP_H__
#define __BOOTPLOOKUP_H__

#include <_types.h>
#include "hostlist.h"

__BEGIN_DECLS

boolean_t	bootp_getbyhw_ds(uint8_t hwtype, void * hwaddr, int hwlen,
				 subnet_match_func_t * func, void * arg,
				 struct in_addr * iaddr_p, 
				 char * * hostname,
				 char * * bootfile);
boolean_t	bootp_getbyip_ds(struct in_addr ciaddr, char * * hostname, 
				 char * * bootfile);

__END_DECLS

#endif	/* __BOOTPLOOKUP_H__ */
