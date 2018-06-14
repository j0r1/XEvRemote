/*

  This file is a part of ErrUt, a small collection of error handling
  utilities.
  
  Copyright (c) 2008-2018 Jori Liesenborgs

  Contact: jori.liesenborgs@gmail.com

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

/**
 * \file errorbase.h
 */

#ifndef ERRUT_ERRORBASE_H

#define ERRUT_ERRORBASE_H

//#include "errutconfig.h"
#include <string>

namespace errut
{
	
/** Base class which allows an error message to be set. 
 *  Base class which allows an error message to be set. Optionally, an
 *  object name can be stored at creation time.
 */
class /*ERRUT_IMPORTEXPORT */ErrorBase
{
public:
	/** Creates an instance without an explicit object name. */
	ErrorBase()														{ m_objectName = std::string("Unnamed object"); }

	/** Creates an instance with the object name set to \c objName. */
	ErrorBase(const std::string &objName)							{ m_objectName = objName; }
	
	virtual ~ErrorBase()											{ }

	/** Returns the stored object name. */
	std::string getObjectName() const								{ return m_objectName; }

	/** Returns the currently stored error message. */
	std::string getErrorString() const								{ return m_errorString; }
protected:
	/** Derived classes can use this member function to store an error message. */
	void setErrorString(const std::string &str) const				{ m_errorString = str; }
private:
	mutable std::string m_errorString;
	std::string m_objectName;
};

} // end namespace

#endif // ERRUT_ERRORBASE_H

