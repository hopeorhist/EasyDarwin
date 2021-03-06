/*
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * Copyright (c) 1999-2008 Apple Inc.  All Rights Reserved.
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
 *
 */
 /*
	 File:       QTSSModuleUtils.h

	 Contains:   Utility routines for modules to use.

 */

#ifndef _QTSS_MODULE_UTILS_H_
#define _QTSS_MODULE_UTILS_H_

#include <stdlib.h>
#include "SafeStdLib.h"
#include "QTSS.h"
#include "StrPtrLen.h"

class QTSSModuleUtils
{
public:
	static void     Initialize(QTSS_TextMessagesObject inMessages,
		QTSS_ServerObject inServer,
		QTSS_StreamRef inErrorLog);

	// Read the complete contents of the file at inPath into the StrPtrLen.
	// This function allocates memory for the file data.
	static QTSS_Error   ReadEntireFile(char* inPath, StrPtrLen* outData, QTSS_TimeVal inModDate = -1, QTSS_TimeVal* outModDate = NULL);

	// Using a message out of the text messages dictionary is a common
	// way to log errors to the error log. Here is a function to
	// make that process very easy.

	static void     LogError(QTSS_ErrorVerbosity inVerbosity,
		QTSS_AttributeID inTextMessage,
		UInt32 inErrNumber,
		char* inArgument = NULL,
		char* inArg2 = NULL);

	static void   LogErrorStr(QTSS_ErrorVerbosity inVerbosity, char* inMessage);
	static void   LogPrefErrorStr(QTSS_ErrorVerbosity inVerbosity, char*  preference, char* inMessage);

	// Called by SendDescribeResponse to coalesce iovec to a buffer
	// Allocates memory - remember to delete it!
	static char* CoalesceVectors(iovec* inVec, UInt32 inNumVectors, UInt32 inTotalLength);

	//
	// SEARCH FOR A SPECIFIC MODULE OBJECT                          
	static QTSS_ModulePrefsObject GetModuleObjectByName(const StrPtrLen& inModuleName);

	//
	// GET MODULE PREFS OBJECT
	static QTSS_ModulePrefsObject GetModulePrefsObject(QTSS_ModuleObject inModObject);

	// GET MODULE ATTRIBUTES OBJECT
	static QTSS_Object GetModuleAttributesObject(QTSS_ModuleObject inModObject);

	//
	// GET ATTRIBUTE
	//
	// This function retrieves an attribute 
	// (from any QTSS_Object, including the QTSS_ModulePrefsObject)
	// with the specified name and type
	// out of the specified object.
	//
	// Caller should pass in a buffer for ioBuffer that is large enough
	// to hold the attribute value. inBufferLen should be set to the length
	// of this buffer.
	//
	// Pass in a buffer containing a default value to use for the attribute
	// in the inDefaultValue parameter. If the attribute isn't found, or is
	// of the wrong type, the default value will be copied into ioBuffer.
	// Also, this function adds the default value to object if it is not
	// found or is of the wrong type. If no default value is provided, the
	// attribute is still added but no value is assigned to it.
	//
	// Pass in NULL for the default value or 0 for the default value length if it is not known.
	//
	// This function logs an error if there was a default value provided.
	static void GetAttribute(QTSS_Object inObject, char* inAttributeName, QTSS_AttrDataType inType,
		void* ioBuffer, void* inDefaultValue, UInt32 inBufferLen);

	static void GetIOAttribute(QTSS_Object inObject, char* inAttributeName, QTSS_AttrDataType inType,
		void* ioDefaultResultBuffer, UInt32 inBufferLen);
	//
	// GET STRING ATTRIBUTE
	//
	// Does the same thing as GetAttribute, but does it for string attribute. Returns a newly
	// allocated buffer with the attribute value inside it.
	//
	// Pass in NULL for the default value or an empty string if the default is not known.
	static char* GetStringAttribute(QTSS_Object inObject, char* inAttributeName, char* inDefaultValue);

	//
	// GET ATTR ID
	//
	// Given an attribute in an object, returns its attribute ID
	// or qtssIllegalAttrID if it isn't found.
	static QTSS_AttributeID GetAttrID(QTSS_Object inObject, char* inAttributeName);

	//
	//
	//
	/// Get the type of request. Returns qtssActionFlagsNoFlags on failure.
	//  Result is a bitmap of flags
	//

	static QTSS_AttrRights GetRights(QTSS_UserProfileObject theUserProfileObject);
	static char* GetExtendedRights(QTSS_UserProfileObject theUserProfileObject, UInt32 index);

	static char*  GetUserName_Copy(QTSS_UserProfileObject inUserProfile);
	static char** GetGroupsArray_Copy(QTSS_UserProfileObject inUserProfile, UInt32 *outNumGroupsPtr);
	static bool UserInGroup(QTSS_UserProfileObject inUserProfile, char* inGroupName, UInt32 inGroupNameLen);

	static QTSS_AttributeID CreateAttribute(QTSS_Object inObject, char* inAttributeName, QTSS_AttrDataType inType, void* inDefaultValue, UInt32 inBufferLen);

	static bool AddressInList(QTSS_Object inObject, QTSS_AttributeID listID, StrPtrLen *theAddressPtr);

	static void SetMisingPrefLogVerbosity(QTSS_ErrorVerbosity verbosityLevel) { QTSSModuleUtils::sMissingPrefVerbosity = verbosityLevel; }
	static QTSS_ErrorVerbosity GetMisingPrefLogVerbosity() { return QTSSModuleUtils::sMissingPrefVerbosity; }

	static bool FindStringInAttributeList(QTSS_Object inObject, QTSS_AttributeID listID, StrPtrLen *inStrPtr);

private:

	//
	// Used in the implementation of the above functions
	static QTSS_AttributeID CheckAttributeDataType(QTSS_Object inObject, char* inAttributeName, QTSS_AttrDataType inType, void* inDefaultValue, UInt32 inBufferLen);

	static QTSS_TextMessagesObject  sMessages;
	static QTSS_ServerObject        sServer;
	static QTSS_StreamRef           sErrorLog;
	static QTSS_ErrorVerbosity      sMissingPrefVerbosity;
};


class IPComponentStr
{
public:
	enum { kNumComponents = 4 };

	StrPtrLen   fAddressComponent[kNumComponents];
	bool      fIsValid;
	static IPComponentStr sLocalIPCompStr;

	IPComponentStr() : fIsValid(false) {}
	IPComponentStr(char *theAddress);
	IPComponentStr(StrPtrLen *sourceStrPtr);

	inline  StrPtrLen*  GetComponent(UInt16 which);
	bool      Equal(IPComponentStr *testAddressPtr);
	bool      Set(StrPtrLen *theAddressStrPtr);
	bool      Valid() { return fIsValid; }
	inline  bool      IsLocal();

};


bool  IPComponentStr::IsLocal()
{
	if (this->Equal(&sLocalIPCompStr))
		return true;

	return false;
}

StrPtrLen* IPComponentStr::GetComponent(UInt16 which)
{
	if (which < IPComponentStr::kNumComponents)
		return &fAddressComponent[which];

	Assert(0);
	return NULL;
}

#endif //_QTSS_MODULE_UTILS_H_
