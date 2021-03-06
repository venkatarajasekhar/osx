/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:  
 *       "This product includes software developed by the 
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "The Jakarta Project", "Tomcat", and "Apache Software
 *    Foundation" must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 */
package javax.servlet.jsp.tagext;

import javax.servlet.jsp.*;
import javax.servlet.jsp.tagext.*;

import javax.servlet.*;

import java.io.Writer;

import java.util.Hashtable;

/**
 * A base class for defining tag handlers implementing BodyTag.
 *
 * <p>
 * The BodyTagSupport class implements the BodyTag interface and adds
 * additional convenience methods including getter methods for the
 * bodyContent property and methods to get at the previous out JspWriter.
 *
 * <p>
 * Many tag handlers will extend BodyTagSupport and only redefine a
 * few methods.
 */

public class BodyTagSupport extends TagSupport implements BodyTag {

    /**
     * Default constructor, all subclasses are required to only define
     * a public constructor with the same signature, and to call the
     * superclass constructor.
     *
     * This constructor is called by the code generated by the JSP
     * translator.
     */

    public BodyTagSupport() {
	super();
    }

    /**
     * Default processing of the start tag returning EVAL_BODY_BUFFERED
     *
     * @return EVAL_BODY_BUFFERED;
     * @seealso BodyTag#doStartTag
     */
 
    public int doStartTag() throws JspException {
        return EVAL_BODY_BUFFERED;
    }


    /**
     * Default processing of the end tag returning EVAL_PAGE.
     *
     * @return EVAL_PAGE
     * @seealso Tag#doEndTag
     */

    public int doEndTag() throws JspException {
	return super.doEndTag();
    }


    // Actions related to body evaluation

    /**
     * Prepare for evaluation of the body: stash the bodyContent away.
     *
     * @param b the BodyContent
     * @seealso #doAfterBody
     * @seealso #doInitBody()
     * @seealso BodyTag#setBodyContent
     */

    public void setBodyContent(BodyContent b) {
	this.bodyContent = b;
    }


    /**
     * Prepare for evaluation of the body just before the first body evaluation:
     * no action.
     *
     * @seealso #setBodyContent
     * @seealso #doAfterBody
     * @seealso BodyTag#doInitBody
     */

    public void doInitBody() throws JspException {
    }


    /**
     * After the body evaluation: do not reevaluate and continue with the page.
     * By default nothing is done with the bodyContent data (if any).
     *
     * @return SKIP_BODY
     * @seealso #doInitBody
     * @seealso BodyTag#doAfterBody
     */

    public int doAfterBody() throws JspException {
 	return SKIP_BODY;
    }


    /**
     * Release state.
     *
     * @seealso Tag#release
     */

    public void release() {
	bodyContent = null;

	super.release();
    }

    /**
     * Get current bodyContent.
     *
     * @return the body content.
     */
    
    public BodyContent getBodyContent() {
	return bodyContent;
    }


    /**
     * Get surrounding out JspWriter.
     *
     * @return the enclosing JspWriter, from the bodyContent.
     */

    public JspWriter getPreviousOut() {
	return bodyContent.getEnclosingWriter();
    }

    // protected fields

    protected BodyContent   bodyContent;
}
