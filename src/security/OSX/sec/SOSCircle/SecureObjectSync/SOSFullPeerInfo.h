/*
 * Copyright (c) 2012-2014 Apple Inc. All Rights Reserved.
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


#ifndef _SOSFULLPEERINFO_H_
#define _SOSFULLPEERINFO_H_

#include <CoreFoundation/CoreFoundation.h>
#include <Security/SecKey.h>
#include <CommonCrypto/CommonDigestSPI.h>
#include <Security/SecureObjectSync/SOSPeerInfo.h>
#include <Security/SecureObjectSync/SOSCloudCircle.h>

__BEGIN_DECLS

typedef struct __OpaqueSOSFullPeerInfo   *SOSFullPeerInfoRef;

enum {
    kSOSFullPeerVersion = 1,
};

SOSFullPeerInfoRef SOSFullPeerInfoCreate(CFAllocatorRef allocator, CFDictionaryRef gestalt, CFDataRef backupKey, SecKeyRef signingKey, CFErrorRef *error);

bool SOSFullPeerInfoUpdateToThisPeer(SOSFullPeerInfoRef peer, SOSPeerInfoRef pi, CFErrorRef *error);

SOSFullPeerInfoRef SOSFullPeerInfoCreateWithViews(CFAllocatorRef allocator,
                                                  CFDictionaryRef gestalt, CFDataRef backupKey, CFSetRef enabledViews,
                                                  SecKeyRef signingKey, CFErrorRef *error);

SOSFullPeerInfoRef SOSFullPeerInfoCopyFullPeerInfo(SOSFullPeerInfoRef toCopy);

SOSFullPeerInfoRef SOSFullPeerInfoCreateCloudIdentity(CFAllocatorRef allocator, SOSPeerInfoRef peer, CFErrorRef* error);

SOSPeerInfoRef SOSFullPeerInfoGetPeerInfo(SOSFullPeerInfoRef fullPeer);
SecKeyRef      SOSFullPeerInfoCopyDeviceKey(SOSFullPeerInfoRef fullPeer, CFErrorRef* error);

bool SOSFullPeerInfoPurgePersistentKey(SOSFullPeerInfoRef peer, CFErrorRef* error);

SOSPeerInfoRef SOSFullPeerInfoPromoteToRetiredAndCopy(SOSFullPeerInfoRef peer, CFErrorRef *error);

bool SOSFullPeerInfoPing(SOSFullPeerInfoRef peer, CFErrorRef* error);

bool SOSFullPeerInfoValidate(SOSFullPeerInfoRef peer, CFErrorRef* error);

bool SOSFullPeerInfoPrivKeyExists(SOSFullPeerInfoRef peer);

bool SOSFullPeerInfoUpdateGestalt(SOSFullPeerInfoRef peer, CFDictionaryRef gestalt, CFErrorRef* error);

bool SOSFullPeerInfoUpdateBackupKey(SOSFullPeerInfoRef peer, CFDataRef backupKey, CFErrorRef* error);

bool SOSFullPeerInfoAddEscrowRecord(SOSFullPeerInfoRef peer, CFStringRef dsid, CFDictionaryRef escrowRecord, CFErrorRef* error);

bool SOSFullPeerInfoReplaceEscrowRecords(SOSFullPeerInfoRef peer, CFDictionaryRef escrowRecords, CFErrorRef* error);

bool SOSFullPeerInfoUpdateToCurrent(SOSFullPeerInfoRef peer, CFSetRef minimumViews, CFSetRef excludedViews);

SOSViewResultCode SOSFullPeerInfoUpdateViews(SOSFullPeerInfoRef peer, SOSViewActionCode action, CFStringRef viewname, CFErrorRef* error);

SOSViewResultCode SOSFullPeerInfoViewStatus(SOSFullPeerInfoRef peer, CFStringRef viewname, CFErrorRef *error);

bool SOSFullPeerInfoPromoteToApplication(SOSFullPeerInfoRef fpi, SecKeyRef user_key, CFErrorRef *error);

bool SOSFullPeerInfoUpgradeSignatures(SOSFullPeerInfoRef fpi, SecKeyRef user_key, CFErrorRef *error);

//
// DER Import Export
//
SOSFullPeerInfoRef SOSFullPeerInfoCreateFromDER(CFAllocatorRef allocator, CFErrorRef* error,
                                        const uint8_t** der_p, const uint8_t *der_end);

SOSFullPeerInfoRef SOSFullPeerInfoCreateFromData(CFAllocatorRef allocator, CFDataRef fullPeerData, CFErrorRef *error);

size_t      SOSFullPeerInfoGetDEREncodedSize(SOSFullPeerInfoRef peer, CFErrorRef *error);
uint8_t*    SOSFullPeerInfoEncodeToDER(SOSFullPeerInfoRef peer, CFErrorRef* error,
                                   const uint8_t* der, uint8_t* der_end);

CFDataRef SOSFullPeerInfoCopyEncodedData(SOSFullPeerInfoRef peer, CFAllocatorRef allocator, CFErrorRef *error);

bool SOSFullPeerInfoUpdateTransportType(SOSFullPeerInfoRef peer, CFStringRef transportType, CFErrorRef* error);
bool SOSFullPeerInfoUpdateDeviceID(SOSFullPeerInfoRef peer, CFStringRef deviceID, CFErrorRef* error);
bool SOSFullPeerInfoUpdateTransportPreference(SOSFullPeerInfoRef peer, CFBooleanRef preference, CFErrorRef* error);
SOSSecurityPropertyResultCode SOSFullPeerInfoUpdateSecurityProperty(SOSFullPeerInfoRef peer, SOSViewActionCode action, CFStringRef property, CFErrorRef* error);
SOSSecurityPropertyResultCode SOSFullPeerInfoSecurityPropertyStatus(SOSFullPeerInfoRef peer, CFStringRef property, CFErrorRef *error);


__END_DECLS

#endif
