/*
 *  si-81-sectrust-appletv.c
 *  Security
 *
 *  Copyright (c) 2015 Apple Inc. All Rights Reserved.
 *
 */

#include <CoreFoundation/CoreFoundation.h>
#include <Security/SecCertificate.h>
#include <Security/SecCertificatePriv.h>
#include <Security/SecCertificateInternal.h>
#include <Security/SecItem.h>
#include <Security/SecItemPriv.h>
#include <Security/SecIdentityPriv.h>
#include <Security/SecIdentity.h>
#include <Security/SecPolicy.h>
#include <Security/SecPolicyPriv.h>
#include <Security/SecPolicyInternal.h>
#include <Security/SecCMS.h>
#include <utilities/SecCFWrappers.h>
#include <stdlib.h>
#include <unistd.h>

#include "Security_regressions.h"

static const UInt8 kTestAppleTVOSAppSigLeaf[] = {
    0x30, 0x82, 0x05, 0x5f, 0x30, 0x82, 0x04, 0x47, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x08, 0x34, 0xc4, 0xe1, 0x74, 0xfd, 0x82, 0xed, 0x21, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
    0x05, 0x00, 0x30, 0x81, 0x96, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0a, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x2c, 0x30, 0x2a, 0x06, 0x03, 0x55, 0x04, 0x0b,
    0x0c, 0x23, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c,
    0x64, 0x77, 0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f,
    0x70, 0x65, 0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e,
    0x73, 0x31, 0x44, 0x30, 0x42, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x3b,
    0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x77,
    0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65,
    0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20,
    0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f,
    0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x30,
    0x1e, 0x17, 0x0d, 0x31, 0x35, 0x30, 0x36, 0x32, 0x36, 0x32, 0x32, 0x34,
    0x30, 0x31, 0x37, 0x5a, 0x17, 0x0d, 0x32, 0x33, 0x30, 0x32, 0x30, 0x37,
    0x32, 0x31, 0x34, 0x38, 0x34, 0x37, 0x5a, 0x30, 0x4b, 0x31, 0x27, 0x30,
    0x25, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x1e, 0x41, 0x70, 0x70, 0x6c,
    0x65, 0x20, 0x54, 0x56, 0x4f, 0x53, 0x20, 0x41, 0x70, 0x70, 0x6c, 0x69,
    0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x53, 0x69, 0x67, 0x6e, 0x69,
    0x6e, 0x67, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c,
    0x0a, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31,
    0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53,
    0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00,
    0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xba, 0x8f, 0xd0,
    0x2b, 0xfb, 0x04, 0x41, 0x7e, 0xef, 0x73, 0xf1, 0x86, 0x5b, 0xce, 0xe8,
    0x0d, 0xb5, 0xec, 0x5f, 0xd9, 0x24, 0x49, 0x6d, 0x5c, 0x97, 0xeb, 0xb2,
    0xa6, 0xfb, 0x7c, 0x9f, 0xcf, 0xd0, 0x18, 0xfa, 0xa1, 0xdf, 0x9f, 0x4a,
    0x42, 0xc3, 0xc3, 0xd3, 0x46, 0x91, 0x8c, 0x74, 0x3b, 0x6e, 0x54, 0xb8,
    0xe7, 0xec, 0x10, 0x8b, 0xc0, 0x2f, 0xe8, 0x96, 0x86, 0xaa, 0x8b, 0xb7,
    0x8f, 0xee, 0x2a, 0x31, 0xf3, 0xaf, 0x04, 0x77, 0x16, 0x09, 0x9e, 0xf9,
    0x9d, 0x30, 0x74, 0x5d, 0x9e, 0xb1, 0x11, 0x66, 0xef, 0x0d, 0x61, 0x1c,
    0xc2, 0xfe, 0x6b, 0x75, 0x80, 0x0e, 0x42, 0x14, 0x4e, 0xdc, 0x38, 0xfd,
    0x18, 0x22, 0x03, 0xe0, 0x51, 0xbd, 0xd0, 0xf3, 0x52, 0x36, 0xff, 0x83,
    0x90, 0xde, 0xbe, 0x60, 0xec, 0x82, 0x66, 0xad, 0x49, 0x54, 0x71, 0x39,
    0xdd, 0x48, 0xc3, 0x13, 0x99, 0xc2, 0xcc, 0x77, 0x55, 0x5e, 0x48, 0xeb,
    0xee, 0x34, 0x31, 0x04, 0xef, 0x7e, 0xe1, 0x42, 0x54, 0x10, 0xcf, 0x09,
    0x9c, 0x0d, 0xc4, 0x55, 0x3d, 0x30, 0x98, 0x78, 0xfb, 0x38, 0xac, 0xdb,
    0xd8, 0x63, 0x3f, 0x64, 0x07, 0x7f, 0x53, 0x4d, 0xc8, 0xbc, 0x60, 0x3e,
    0x89, 0x49, 0x88, 0x07, 0xb4, 0x80, 0x15, 0xd5, 0xc2, 0x13, 0x8b, 0xff,
    0x0c, 0x90, 0xb6, 0x67, 0x0c, 0xaf, 0xf4, 0xef, 0x5c, 0x9d, 0xba, 0xf3,
    0x95, 0x5b, 0xd2, 0x9a, 0x7e, 0x80, 0x8d, 0xc9, 0x6f, 0xcd, 0x75, 0xe5,
    0xb6, 0xfb, 0x61, 0x8b, 0x9c, 0x3b, 0xce, 0xc2, 0x4c, 0xba, 0xb7, 0xf6,
    0x48, 0xa6, 0x79, 0x4a, 0x34, 0xf1, 0xe1, 0x47, 0xba, 0x29, 0x5d, 0x04,
    0x26, 0x64, 0xee, 0x5e, 0x8e, 0x0c, 0x9d, 0xa7, 0x05, 0xe3, 0x58, 0xd7,
    0xe4, 0xb5, 0x4e, 0x7b, 0xdc, 0x2a, 0xab, 0xc1, 0xea, 0x82, 0x7d, 0xcb,
    0x93, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01, 0xf9, 0x30, 0x82,
    0x01, 0xf5, 0x30, 0x47, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07,
    0x01, 0x01, 0x04, 0x3b, 0x30, 0x39, 0x30, 0x37, 0x06, 0x08, 0x2b, 0x06,
    0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x2b, 0x68, 0x74, 0x74, 0x70,
    0x3a, 0x2f, 0x2f, 0x6f, 0x63, 0x73, 0x70, 0x2e, 0x61, 0x70, 0x70, 0x6c,
    0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x6f, 0x63, 0x73, 0x70, 0x30, 0x34,
    0x2d, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x77, 0x77, 0x64, 0x72, 0x63, 0x61,
    0x32, 0x30, 0x33, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16,
    0x04, 0x14, 0x49, 0xaa, 0xae, 0x84, 0x57, 0x14, 0x56, 0x8f, 0x0b, 0xeb,
    0x63, 0x6b, 0x62, 0x75, 0x68, 0xfc, 0x5b, 0x8c, 0x77, 0xa1, 0x30, 0x0c,
    0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x02, 0x30, 0x00,
    0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80,
    0x14, 0x88, 0x27, 0x17, 0x09, 0xa9, 0xb6, 0x18, 0x60, 0x8b, 0xec, 0xeb,
    0xba, 0xf6, 0x47, 0x59, 0xc5, 0x52, 0x54, 0xa3, 0xb7, 0x30, 0x82, 0x01,
    0x1d, 0x06, 0x03, 0x55, 0x1d, 0x20, 0x04, 0x82, 0x01, 0x14, 0x30, 0x82,
    0x01, 0x10, 0x30, 0x82, 0x01, 0x0c, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x63, 0x64, 0x05, 0x01, 0x30, 0x81, 0xfe, 0x30, 0x81, 0xc3, 0x06,
    0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x02, 0x30, 0x81, 0xb6,
    0x0c, 0x81, 0xb3, 0x52, 0x65, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x20,
    0x6f, 0x6e, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x63, 0x65, 0x72, 0x74,
    0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x62, 0x79, 0x20, 0x61,
    0x6e, 0x79, 0x20, 0x70, 0x61, 0x72, 0x74, 0x79, 0x20, 0x61, 0x73, 0x73,
    0x75, 0x6d, 0x65, 0x73, 0x20, 0x61, 0x63, 0x63, 0x65, 0x70, 0x74, 0x61,
    0x6e, 0x63, 0x65, 0x20, 0x6f, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74,
    0x68, 0x65, 0x6e, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x62,
    0x6c, 0x65, 0x20, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x61, 0x72, 0x64, 0x20,
    0x74, 0x65, 0x72, 0x6d, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x63, 0x6f,
    0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6f, 0x66, 0x20,
    0x75, 0x73, 0x65, 0x2c, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69,
    0x63, 0x61, 0x74, 0x65, 0x20, 0x70, 0x6f, 0x6c, 0x69, 0x63, 0x79, 0x20,
    0x61, 0x6e, 0x64, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63,
    0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x70, 0x72, 0x61, 0x63, 0x74, 0x69,
    0x63, 0x65, 0x20, 0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e, 0x74,
    0x73, 0x2e, 0x30, 0x36, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07,
    0x02, 0x01, 0x16, 0x2a, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77,
    0x77, 0x77, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d,
    0x2f, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65,
    0x61, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x2f, 0x30, 0x0e,
    0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02,
    0x07, 0x80, 0x30, 0x16, 0x06, 0x03, 0x55, 0x1d, 0x25, 0x01, 0x01, 0xff,
    0x04, 0x0c, 0x30, 0x0a, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07,
    0x03, 0x03, 0x30, 0x13, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x63,
    0x64, 0x06, 0x01, 0x18, 0x01, 0x01, 0xff, 0x04, 0x02, 0x05, 0x00, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
    0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x9c, 0x07, 0xde, 0xde, 0xc2,
    0xfc, 0x6c, 0x94, 0xb1, 0x1a, 0x6a, 0x38, 0x75, 0xe0, 0x74, 0x70, 0xe9,
    0x9d, 0x47, 0xd6, 0xde, 0xcd, 0xd0, 0xdb, 0xed, 0x2f, 0x50, 0xfa, 0x0d,
    0xe3, 0xb9, 0x3d, 0x36, 0xc9, 0x4b, 0xee, 0x4e, 0xc4, 0x83, 0xb9, 0x7d,
    0x40, 0x01, 0x92, 0x3f, 0x18, 0x8a, 0x19, 0xe8, 0xac, 0x5d, 0xb1, 0xc1,
    0xd2, 0x30, 0x98, 0x85, 0x28, 0x91, 0x0c, 0x92, 0x71, 0x79, 0xec, 0x4b,
    0x51, 0xcc, 0xdf, 0x99, 0x71, 0x87, 0x04, 0x60, 0x09, 0x3e, 0xfa, 0x56,
    0x9f, 0x99, 0xa3, 0xef, 0x0c, 0x02, 0xd2, 0xdf, 0xcf, 0x18, 0xf2, 0x34,
    0x6e, 0x93, 0xd0, 0x0e, 0x81, 0xe4, 0x4e, 0x37, 0x7b, 0x1d, 0xe7, 0x8c,
    0xa6, 0x71, 0x6d, 0x95, 0x66, 0x7d, 0xc0, 0x80, 0x74, 0x71, 0xe1, 0xd7,
    0x97, 0x35, 0x9b, 0x26, 0xe9, 0x84, 0x4a, 0x96, 0x30, 0xfc, 0xf1, 0x26,
    0x23, 0x1d, 0xec, 0x71, 0x2f, 0x39, 0x40, 0x14, 0xaf, 0x34, 0x0e, 0x85,
    0x3c, 0xd0, 0x9e, 0x8d, 0x4e, 0xf8, 0x04, 0x0a, 0xc2, 0x3f, 0x44, 0x7d,
    0x19, 0x2d, 0xe7, 0xc0, 0xf1, 0xce, 0xa9, 0x2f, 0x6c, 0x79, 0xbd, 0x65,
    0x69, 0x3e, 0xf6, 0x76, 0x59, 0xeb, 0x70, 0x0c, 0xaf, 0x04, 0x44, 0x82,
    0x02, 0x15, 0x24, 0x3e, 0xc3, 0xe0, 0x9e, 0x5d, 0xa0, 0xe3, 0x66, 0x72,
    0x59, 0x6e, 0x51, 0x41, 0xd6, 0x72, 0xdd, 0x4d, 0xca, 0x96, 0xb0, 0x1a,
    0xc1, 0x47, 0x5a, 0xef, 0xc9, 0xc4, 0x11, 0x11, 0x7a, 0xec, 0x9c, 0x1c,
    0x12, 0x19, 0x72, 0xb8, 0xc3, 0x98, 0x3e, 0x3b, 0xe7, 0x4a, 0x3f, 0xb8,
    0x48, 0x40, 0xd6, 0x68, 0xa9, 0xce, 0x07, 0xe7, 0x0e, 0x5e, 0x56, 0x33,
    0xf8, 0xb0, 0x4c, 0xc2, 0xb6, 0x25, 0xcc, 0x5f, 0xbd, 0xdb, 0xe5, 0x78,
    0xb6, 0x5f, 0x99, 0x3e, 0xdc, 0xaf, 0x20, 0x3d, 0x5a, 0x0f, 0x13
};

static const UInt8 kTestAppleTVOSAppSigTestLeaf[] = {
    0x30, 0x82, 0x05, 0x52, 0x30, 0x82, 0x04, 0x3a, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x08, 0x29, 0x7b, 0x51, 0x36, 0x47, 0xa4, 0x6f, 0x23, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
    0x05, 0x00, 0x30, 0x81, 0x96, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0a, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x2c, 0x30, 0x2a, 0x06, 0x03, 0x55, 0x04, 0x0b,
    0x0c, 0x23, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c,
    0x64, 0x77, 0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f,
    0x70, 0x65, 0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e,
    0x73, 0x31, 0x44, 0x30, 0x42, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x3b,
    0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x77,
    0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65,
    0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20,
    0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f,
    0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x30,
    0x1e, 0x17, 0x0d, 0x31, 0x35, 0x30, 0x32, 0x30, 0x36, 0x32, 0x33, 0x35,
    0x33, 0x31, 0x36, 0x5a, 0x17, 0x0d, 0x32, 0x33, 0x30, 0x32, 0x30, 0x37,
    0x32, 0x31, 0x34, 0x38, 0x34, 0x37, 0x5a, 0x30, 0x55, 0x31, 0x31, 0x30,
    0x2f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x28, 0x54, 0x45, 0x53, 0x54,
    0x20, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x54, 0x56, 0x4f, 0x53, 0x20,
    0x41, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20,
    0x53, 0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x20, 0x54, 0x45, 0x53, 0x54,
    0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0a, 0x41,
    0x70, 0x70, 0x6c, 0x65, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x30, 0x82,
    0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82,
    0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xc9, 0x74, 0x71, 0x4a, 0x58,
    0x65, 0xdf, 0x19, 0x27, 0x08, 0x97, 0x9b, 0xf3, 0x12, 0x14, 0x8e, 0xa2,
    0xd0, 0xa2, 0x1e, 0x1d, 0x46, 0xae, 0xdf, 0xc4, 0xef, 0x57, 0xc0, 0x82,
    0x5f, 0xb9, 0xe5, 0x63, 0x53, 0x57, 0xad, 0xaa, 0x32, 0x84, 0x6f, 0xbe,
    0xdf, 0x65, 0x1f, 0x73, 0x0a, 0x85, 0x55, 0x3a, 0xb3, 0xcf, 0x43, 0x02,
    0x18, 0xe4, 0xad, 0x04, 0xa0, 0x83, 0x89, 0x3d, 0x6f, 0xfa, 0xdf, 0xb3,
    0x82, 0xa2, 0xb2, 0x6d, 0x46, 0x63, 0x4d, 0x88, 0x0a, 0xe7, 0x96, 0x68,
    0x3b, 0x6f, 0x96, 0xf8, 0xa9, 0x92, 0x18, 0x15, 0x0d, 0xf4, 0xe9, 0x44,
    0xf5, 0x62, 0xf1, 0x50, 0x4d, 0x86, 0x60, 0x5b, 0x89, 0x72, 0x3c, 0x53,
    0x8a, 0xda, 0x3a, 0x4f, 0x1d, 0x58, 0x1a, 0xc2, 0xaf, 0x46, 0x0c, 0x6d,
    0x53, 0x6d, 0xa3, 0x4d, 0x36, 0xa0, 0xfe, 0x54, 0xc6, 0xdd, 0x94, 0x01,
    0x43, 0xc1, 0xdf, 0x62, 0xd2, 0x2e, 0x76, 0x96, 0x10, 0x29, 0x30, 0x4f,
    0x51, 0x35, 0x5d, 0x5f, 0x10, 0x32, 0x0f, 0xec, 0xad, 0xd0, 0x0a, 0xc1,
    0xde, 0x7f, 0x7d, 0xcc, 0xa7, 0x4b, 0x67, 0x5e, 0x97, 0xbf, 0x45, 0x9f,
    0x0b, 0x68, 0x93, 0x0b, 0x42, 0x7b, 0x49, 0xf9, 0xda, 0x3d, 0xa3, 0x5e,
    0x22, 0x6b, 0x48, 0x2d, 0x86, 0x96, 0x25, 0xc1, 0x78, 0x11, 0xad, 0x7f,
    0x70, 0x43, 0x49, 0x05, 0x8d, 0x59, 0xe2, 0x80, 0x51, 0x79, 0x58, 0x5c,
    0xfb, 0x75, 0x6c, 0xa0, 0x7f, 0x62, 0xf5, 0x7d, 0xc1, 0xe7, 0xf8, 0x06,
    0x85, 0x9f, 0xb3, 0xaa, 0x90, 0x98, 0x53, 0x8d, 0x7b, 0x40, 0x04, 0x71,
    0xf4, 0xa4, 0xce, 0xa0, 0x20, 0x3d, 0x77, 0x32, 0xf5, 0x94, 0x20, 0x54,
    0xa2, 0xe2, 0x98, 0x8c, 0x38, 0x63, 0x94, 0xe5, 0x73, 0xa1, 0xcc, 0xcc,
    0xe4, 0x11, 0x34, 0xfb, 0xff, 0x41, 0x63, 0x2c, 0x39, 0xaf, 0x39, 0x02,
    0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01, 0xe2, 0x30, 0x82, 0x01, 0xde,
    0x30, 0x47, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01,
    0x04, 0x3b, 0x30, 0x39, 0x30, 0x37, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05,
    0x05, 0x07, 0x30, 0x01, 0x86, 0x2b, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f,
    0x2f, 0x6f, 0x63, 0x73, 0x70, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e,
    0x63, 0x6f, 0x6d, 0x2f, 0x6f, 0x63, 0x73, 0x70, 0x30, 0x34, 0x2d, 0x61,
    0x70, 0x70, 0x6c, 0x65, 0x77, 0x77, 0x64, 0x72, 0x63, 0x61, 0x32, 0x30,
    0x34, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14,
    0x0a, 0x14, 0xfb, 0x9f, 0x6f, 0x4e, 0x79, 0xc0, 0xbb, 0xc8, 0xa5, 0x35,
    0xeb, 0x06, 0x6a, 0xe7, 0x45, 0x6a, 0x61, 0xad, 0x30, 0x0c, 0x06, 0x03,
    0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x02, 0x30, 0x00, 0x30, 0x1f,
    0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x88,
    0x27, 0x17, 0x09, 0xa9, 0xb6, 0x18, 0x60, 0x8b, 0xec, 0xeb, 0xba, 0xf6,
    0x47, 0x59, 0xc5, 0x52, 0x54, 0xa3, 0xb7, 0x30, 0x82, 0x01, 0x1d, 0x06,
    0x03, 0x55, 0x1d, 0x20, 0x04, 0x82, 0x01, 0x14, 0x30, 0x82, 0x01, 0x10,
    0x30, 0x82, 0x01, 0x0c, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x63,
    0x64, 0x05, 0x01, 0x30, 0x81, 0xfe, 0x30, 0x81, 0xc3, 0x06, 0x08, 0x2b,
    0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x02, 0x30, 0x81, 0xb6, 0x0c, 0x81,
    0xb3, 0x52, 0x65, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x20, 0x6f, 0x6e,
    0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66,
    0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x62, 0x79, 0x20, 0x61, 0x6e, 0x79,
    0x20, 0x70, 0x61, 0x72, 0x74, 0x79, 0x20, 0x61, 0x73, 0x73, 0x75, 0x6d,
    0x65, 0x73, 0x20, 0x61, 0x63, 0x63, 0x65, 0x70, 0x74, 0x61, 0x6e, 0x63,
    0x65, 0x20, 0x6f, 0x66, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x68, 0x65,
    0x6e, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x62, 0x6c, 0x65,
    0x20, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x61, 0x72, 0x64, 0x20, 0x74, 0x65,
    0x72, 0x6d, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x63, 0x6f, 0x6e, 0x64,
    0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20, 0x6f, 0x66, 0x20, 0x75, 0x73,
    0x65, 0x2c, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61,
    0x74, 0x65, 0x20, 0x70, 0x6f, 0x6c, 0x69, 0x63, 0x79, 0x20, 0x61, 0x6e,
    0x64, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
    0x69, 0x6f, 0x6e, 0x20, 0x70, 0x72, 0x61, 0x63, 0x74, 0x69, 0x63, 0x65,
    0x20, 0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x73, 0x2e,
    0x30, 0x36, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x01,
    0x16, 0x2a, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77,
    0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x63,
    0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x61, 0x75,
    0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x2f, 0x30, 0x0e, 0x06, 0x03,
    0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x07, 0x80,
    0x30, 0x14, 0x06, 0x0b, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x63, 0x64, 0x06,
    0x01, 0x18, 0x01, 0x01, 0x01, 0xff, 0x04, 0x02, 0x05, 0x00, 0x30, 0x0d,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05,
    0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x58, 0xef, 0x32, 0x6c, 0x48, 0x29,
    0xfa, 0x5e, 0x5e, 0x32, 0xa6, 0xbe, 0xe4, 0xd2, 0x3e, 0x72, 0xcf, 0xb9,
    0x74, 0x62, 0x84, 0x90, 0xa1, 0x5f, 0xbb, 0xd3, 0x3d, 0x67, 0x19, 0xf4,
    0x1b, 0xa1, 0x31, 0x38, 0xe0, 0xdb, 0xe4, 0x14, 0x6d, 0x9e, 0x99, 0x34,
    0xd3, 0x53, 0x97, 0xb4, 0xaa, 0x63, 0x61, 0x56, 0xac, 0x1e, 0x70, 0x54,
    0x98, 0x18, 0x2d, 0xc9, 0xa8, 0x31, 0x21, 0x95, 0x64, 0x25, 0xc1, 0x3e,
    0xfa, 0xbb, 0xc8, 0x13, 0x9b, 0x0c, 0xa5, 0xa5, 0xc2, 0x8e, 0x4e, 0xad,
    0x25, 0xef, 0xbe, 0x94, 0xe6, 0x0e, 0x91, 0x36, 0x44, 0xad, 0x93, 0x12,
    0x20, 0x3c, 0x3a, 0xc0, 0xfe, 0x6d, 0x47, 0xbe, 0xa1, 0x29, 0xde, 0x53,
    0xee, 0x6c, 0xee, 0x56, 0xec, 0xae, 0xeb, 0x08, 0x24, 0x3e, 0x43, 0xef,
    0x92, 0x6b, 0x2a, 0x66, 0x5c, 0x9f, 0x25, 0x77, 0x4e, 0x96, 0x45, 0x4d,
    0xd7, 0xac, 0xc0, 0xc8, 0xfe, 0xd2, 0x37, 0x52, 0xc8, 0xcb, 0xe3, 0x26,
    0xad, 0xb2, 0xd9, 0x90, 0x3f, 0x68, 0x93, 0xb5, 0x3f, 0x10, 0xd3, 0x61,
    0xb7, 0x09, 0x35, 0x42, 0xd4, 0xf4, 0xde, 0x3b, 0x42, 0x3e, 0x8c, 0xe1,
    0xe8, 0xa7, 0xcb, 0x24, 0x2c, 0x38, 0xd1, 0xa0, 0x99, 0x22, 0xd9, 0xab,
    0x3a, 0x39, 0xda, 0x78, 0x22, 0x2a, 0x01, 0xe2, 0xda, 0x30, 0x0b, 0x82,
    0xca, 0x7d, 0xe0, 0xca, 0xd0, 0x95, 0x13, 0x50, 0x4f, 0x85, 0x86, 0x83,
    0x3d, 0x3d, 0xa2, 0x2c, 0xeb, 0x46, 0x7c, 0x50, 0xc0, 0x5a, 0x60, 0x7b,
    0x70, 0xb5, 0x5f, 0xb7, 0xa8, 0x54, 0x81, 0xe7, 0xb0, 0xf2, 0x91, 0xc6,
    0xd6, 0xc1, 0xc4, 0xd6, 0xdb, 0xea, 0xfa, 0xf4, 0xf0, 0x6c, 0x00, 0xbf,
    0x0f, 0x71, 0xff, 0xb3, 0x6c, 0x59, 0x08, 0x2f, 0x28, 0xd3, 0xaf, 0xc3,
    0xd2, 0xde, 0xe1, 0x1a, 0x54, 0x76, 0xfe, 0x2c, 0x98, 0xf1
};

static const UInt8 kTestAppleWWDRIntm[] = {
    0x30, 0x82, 0x04, 0x23, 0x30, 0x82, 0x03, 0x0b, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x19, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x62, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13,
    0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0a, 0x41, 0x70, 0x70,
    0x6c, 0x65, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x26, 0x30, 0x24, 0x06,
    0x03, 0x55, 0x04, 0x0b, 0x13, 0x1d, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20,
    0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f,
    0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x31,
    0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x0d, 0x41, 0x70,
    0x70, 0x6c, 0x65, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x20, 0x43, 0x41, 0x30,
    0x1e, 0x17, 0x0d, 0x30, 0x38, 0x30, 0x32, 0x31, 0x34, 0x31, 0x38, 0x35,
    0x36, 0x33, 0x35, 0x5a, 0x17, 0x0d, 0x31, 0x36, 0x30, 0x32, 0x31, 0x34,
    0x31, 0x38, 0x35, 0x36, 0x33, 0x35, 0x5a, 0x30, 0x81, 0x96, 0x31, 0x0b,
    0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31,
    0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0a, 0x41, 0x70,
    0x70, 0x6c, 0x65, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x2c, 0x30, 0x2a,
    0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x23, 0x41, 0x70, 0x70, 0x6c, 0x65,
    0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x77, 0x69, 0x64, 0x65, 0x20, 0x44,
    0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65, 0x72, 0x20, 0x52, 0x65, 0x6c,
    0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x31, 0x44, 0x30, 0x42, 0x06, 0x03,
    0x55, 0x04, 0x03, 0x0c, 0x3b, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57,
    0x6f, 0x72, 0x6c, 0x64, 0x77, 0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76,
    0x65, 0x6c, 0x6f, 0x70, 0x65, 0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74,
    0x69, 0x6f, 0x6e, 0x73, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69,
    0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f,
    0x72, 0x69, 0x74, 0x79, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09,
    0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03,
    0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01,
    0x00, 0xca, 0x38, 0x54, 0xa6, 0xcb, 0x56, 0xaa, 0xc8, 0x24, 0x39, 0x48,
    0xe9, 0x8c, 0xee, 0xec, 0x5f, 0xb8, 0x7f, 0x26, 0x91, 0xbc, 0x34, 0x53,
    0x7a, 0xce, 0x7c, 0x63, 0x80, 0x61, 0x77, 0x64, 0x5e, 0xa5, 0x07, 0x23,
    0xb6, 0x39, 0xfe, 0x50, 0x2d, 0x15, 0x56, 0x58, 0x70, 0x2d, 0x7e, 0xc4,
    0x6e, 0xc1, 0x4a, 0x85, 0x3e, 0x2f, 0xf0, 0xde, 0x84, 0x1a, 0xa1, 0x57,
    0xc9, 0xaf, 0x7b, 0x18, 0xff, 0x6a, 0xfa, 0x15, 0x12, 0x49, 0x15, 0x08,
    0x19, 0xac, 0xaa, 0xdb, 0x2a, 0x32, 0xed, 0x96, 0x63, 0x68, 0x52, 0x15,
    0x3d, 0x8c, 0x8a, 0xec, 0xbf, 0x6b, 0x18, 0x95, 0xe0, 0x03, 0xac, 0x01,
    0x7d, 0x97, 0x05, 0x67, 0xce, 0x0e, 0x85, 0x95, 0x37, 0x6a, 0xed, 0x09,
    0xb6, 0xae, 0x67, 0xcd, 0x51, 0x64, 0x9f, 0xc6, 0x5c, 0xd1, 0xbc, 0x57,
    0x6e, 0x67, 0x35, 0x80, 0x76, 0x36, 0xa4, 0x87, 0x81, 0x6e, 0x38, 0x8f,
    0xd8, 0x2b, 0x15, 0x4e, 0x7b, 0x25, 0xd8, 0x5a, 0xbf, 0x4e, 0x83, 0xc1,
    0x8d, 0xd2, 0x93, 0xd5, 0x1a, 0x71, 0xb5, 0x60, 0x9c, 0x9d, 0x33, 0x4e,
    0x55, 0xf9, 0x12, 0x58, 0x0c, 0x86, 0xb8, 0x16, 0x0d, 0xc1, 0xe5, 0x77,
    0x45, 0x8d, 0x50, 0x48, 0xba, 0x2b, 0x2d, 0xe4, 0x94, 0x85, 0xe1, 0xe8,
    0xc4, 0x9d, 0xc6, 0x68, 0xa5, 0xb0, 0xa3, 0xfc, 0x67, 0x7e, 0x70, 0xba,
    0x02, 0x59, 0x4b, 0x77, 0x42, 0x91, 0x39, 0xb9, 0xf5, 0xcd, 0xe1, 0x4c,
    0xef, 0xc0, 0x3b, 0x48, 0x8c, 0xa6, 0xe5, 0x21, 0x5d, 0xfd, 0x6a, 0x6a,
    0xbb, 0xa7, 0x16, 0x35, 0x60, 0xd2, 0xe6, 0xad, 0xf3, 0x46, 0x29, 0xc9,
    0xe8, 0xc3, 0x8b, 0xe9, 0x79, 0xc0, 0x6a, 0x61, 0x67, 0x15, 0xb2, 0xf0,
    0xfd, 0xe5, 0x68, 0xbc, 0x62, 0x5f, 0x6e, 0xcf, 0x99, 0xdd, 0xef, 0x1b,
    0x63, 0xfe, 0x92, 0x65, 0xab, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x81,
    0xae, 0x30, 0x81, 0xab, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01,
    0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x0f, 0x06, 0x03,
    0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01,
    0xff, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14,
    0x88, 0x27, 0x17, 0x09, 0xa9, 0xb6, 0x18, 0x60, 0x8b, 0xec, 0xeb, 0xba,
    0xf6, 0x47, 0x59, 0xc5, 0x52, 0x54, 0xa3, 0xb7, 0x30, 0x1f, 0x06, 0x03,
    0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x2b, 0xd0, 0x69,
    0x47, 0x94, 0x76, 0x09, 0xfe, 0xf4, 0x6b, 0x8d, 0x2e, 0x40, 0xa6, 0xf7,
    0x47, 0x4d, 0x7f, 0x08, 0x5e, 0x30, 0x36, 0x06, 0x03, 0x55, 0x1d, 0x1f,
    0x04, 0x2f, 0x30, 0x2d, 0x30, 0x2b, 0xa0, 0x29, 0xa0, 0x27, 0x86, 0x25,
    0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x61,
    0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x61, 0x70, 0x70,
    0x6c, 0x65, 0x63, 0x61, 0x2f, 0x72, 0x6f, 0x6f, 0x74, 0x2e, 0x63, 0x72,
    0x6c, 0x30, 0x10, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x63, 0x64,
    0x06, 0x02, 0x01, 0x04, 0x02, 0x05, 0x00, 0x30, 0x0d, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x82,
    0x01, 0x01, 0x00, 0xda, 0x32, 0x00, 0x96, 0xc5, 0x54, 0x94, 0xd3, 0x3b,
    0x82, 0x37, 0x66, 0x7d, 0x2e, 0x68, 0xd5, 0xc3, 0xc6, 0xb8, 0xcb, 0x26,
    0x8c, 0x48, 0x90, 0xcf, 0x13, 0x24, 0x6a, 0x46, 0x8e, 0x63, 0xd4, 0xf0,
    0xd0, 0x13, 0x06, 0xdd, 0xd8, 0xc4, 0xc1, 0x37, 0x15, 0xf2, 0x33, 0x13,
    0x39, 0x26, 0x2d, 0xce, 0x2e, 0x55, 0x40, 0xe3, 0x0b, 0x03, 0xaf, 0xfa,
    0x12, 0xc2, 0xe7, 0x0d, 0x21, 0xb8, 0xd5, 0x80, 0xcf, 0xac, 0x28, 0x2f,
    0xce, 0x2d, 0xb3, 0x4e, 0xaf, 0x86, 0x19, 0x04, 0xc6, 0xe9, 0x50, 0xdd,
    0x4c, 0x29, 0x47, 0x10, 0x23, 0xfc, 0x6c, 0xbb, 0x1b, 0x98, 0x6b, 0x48,
    0x89, 0xe1, 0x5b, 0x9d, 0xde, 0x46, 0xdb, 0x35, 0x85, 0x35, 0xef, 0x3e,
    0xd0, 0xe2, 0x58, 0x4b, 0x38, 0xf4, 0xed, 0x75, 0x5a, 0x1f, 0x5c, 0x70,
    0x1d, 0x56, 0x39, 0x12, 0xe5, 0xe1, 0x0d, 0x11, 0xe4, 0x89, 0x25, 0x06,
    0xbd, 0xd5, 0xb4, 0x15, 0x8e, 0x5e, 0xd0, 0x59, 0x97, 0x90, 0xe9, 0x4b,
    0x81, 0xe2, 0xdf, 0x18, 0xaf, 0x44, 0x74, 0x1e, 0x19, 0xa0, 0x3a, 0x47,
    0xcc, 0x91, 0x1d, 0x3a, 0xeb, 0x23, 0x5a, 0xfe, 0xa5, 0x2d, 0x97, 0xf7,
    0x7b, 0xbb, 0xd6, 0x87, 0x46, 0x42, 0x85, 0xeb, 0x52, 0x3d, 0x26, 0xb2,
    0x63, 0xa8, 0xb4, 0xb1, 0xca, 0x8f, 0xf4, 0xcc, 0xe2, 0xb3, 0xc8, 0x47,
    0xe0, 0xbf, 0x9a, 0x59, 0x83, 0xfa, 0xda, 0x98, 0x53, 0x2a, 0x82, 0xf5,
    0x7c, 0x65, 0x2e, 0x95, 0xd9, 0x33, 0x5d, 0xf5, 0xed, 0x65, 0xcc, 0x31,
    0x37, 0xc5, 0x5a, 0x04, 0xe8, 0x6b, 0xe1, 0xe7, 0x88, 0x03, 0x4a, 0x75,
    0x9e, 0x9b, 0x28, 0xcb, 0x4a, 0x40, 0x88, 0x65, 0x43, 0x75, 0xdd, 0xcb,
    0x3a, 0x25, 0x23, 0xc5, 0x9e, 0x57, 0xf8, 0x2e, 0xce, 0xd2, 0xa9, 0x92,
    0x5e, 0x73, 0x2e, 0x2f, 0x25, 0x75, 0x15
};

/*  Subject: UID=AP773K8VXL, CN=iPhone Developer: Julien Oster (S64MW3JM4L), OU=M2657GZ2M9, O=Apple Inc. - OS Security, C=US
 *  Issuer: C=US, O=Apple Inc., OU=Apple Worldwide Developer Relations, CN=Apple Worldwide Developer Relations Certification Authority */
static const UInt8 kTestiPhoneDevCert[] = {
    0x30, 0x82, 0x05, 0xa4, 0x30, 0x82, 0x04, 0x8c, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x08, 0x38, 0x3a, 0x38, 0x67, 0x79, 0xb5, 0xa4, 0xc8, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05,
    0x05, 0x00, 0x30, 0x81, 0x96, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0a, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x2c, 0x30, 0x2a, 0x06, 0x03, 0x55, 0x04, 0x0b,
    0x0c, 0x23, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c,
    0x64, 0x77, 0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f,
    0x70, 0x65, 0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e,
    0x73, 0x31, 0x44, 0x30, 0x42, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x3b,
    0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x77,
    0x69, 0x64, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65,
    0x72, 0x20, 0x52, 0x65, 0x6c, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20,
    0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f,
    0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x30,
    0x1e, 0x17, 0x0d, 0x31, 0x34, 0x31, 0x32, 0x31, 0x36, 0x32, 0x31, 0x31,
    0x38, 0x30, 0x30, 0x5a, 0x17, 0x0d, 0x31, 0x35, 0x31, 0x32, 0x31, 0x36,
    0x32, 0x31, 0x31, 0x38, 0x30, 0x30, 0x5a, 0x30, 0x81, 0x97, 0x31, 0x1a,
    0x30, 0x18, 0x06, 0x0a, 0x09, 0x92, 0x26, 0x89, 0x93, 0xf2, 0x2c, 0x64,
    0x01, 0x01, 0x0c, 0x0a, 0x41, 0x50, 0x37, 0x37, 0x33, 0x4b, 0x38, 0x56,
    0x58, 0x4c, 0x31, 0x34, 0x30, 0x32, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
    0x2b, 0x69, 0x50, 0x68, 0x6f, 0x6e, 0x65, 0x20, 0x44, 0x65, 0x76, 0x65,
    0x6c, 0x6f, 0x70, 0x65, 0x72, 0x3a, 0x20, 0x4a, 0x75, 0x6c, 0x69, 0x65,
    0x6e, 0x20, 0x4f, 0x73, 0x74, 0x65, 0x72, 0x20, 0x28, 0x53, 0x36, 0x34,
    0x4d, 0x57, 0x33, 0x4a, 0x4d, 0x34, 0x4c, 0x29, 0x31, 0x13, 0x30, 0x11,
    0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x0a, 0x4d, 0x32, 0x36, 0x35, 0x37,
    0x47, 0x5a, 0x32, 0x4d, 0x39, 0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55,
    0x04, 0x0a, 0x0c, 0x18, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49, 0x6e,
    0x63, 0x2e, 0x20, 0x2d, 0x20, 0x4f, 0x53, 0x20, 0x53, 0x65, 0x63, 0x75,
    0x72, 0x69, 0x74, 0x79, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x06, 0x13, 0x02, 0x55, 0x53, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06,
    0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00,
    0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01,
    0x01, 0x00, 0xaf, 0x63, 0x4c, 0x3e, 0xbd, 0x24, 0xb8, 0xfe, 0xd1, 0xb7,
    0x7e, 0xe6, 0x65, 0x91, 0xf6, 0xcf, 0xd6, 0x5f, 0xe2, 0xf5, 0xe4, 0x75,
    0x22, 0xa4, 0xa7, 0xcd, 0x0e, 0x4a, 0x93, 0xbd, 0x26, 0x19, 0xa0, 0x55,
    0x56, 0x58, 0xbf, 0x82, 0xb3, 0xc7, 0x69, 0xf1, 0x9b, 0x0b, 0xe8, 0xa6,
    0xef, 0xa4, 0x85, 0xcb, 0x17, 0x15, 0xa1, 0x6d, 0x9f, 0xa8, 0x7e, 0xec,
    0x7f, 0x45, 0x87, 0x26, 0x0a, 0x85, 0x32, 0x71, 0x5b, 0x49, 0x62, 0x92,
    0x96, 0xa6, 0x27, 0xe4, 0x1e, 0x33, 0x0a, 0xeb, 0x15, 0x31, 0xb3, 0x31,
    0x88, 0x99, 0x73, 0x27, 0xf5, 0x1e, 0x2b, 0x33, 0xfd, 0x84, 0x71, 0xb0,
    0xd4, 0x84, 0xf1, 0x0b, 0x44, 0x49, 0x6a, 0x41, 0xd1, 0xc3, 0xa1, 0x4d,
    0x18, 0xa7, 0xbb, 0x3c, 0xef, 0x80, 0xc6, 0x28, 0x14, 0x79, 0x2e, 0x6c,
    0x99, 0xd7, 0x10, 0xd9, 0x10, 0xf5, 0xe4, 0xf1, 0x92, 0x87, 0x13, 0x21,
    0x55, 0xa7, 0x1c, 0x90, 0xa1, 0xbb, 0x77, 0xbd, 0xee, 0xc4, 0x14, 0x35,
    0xfe, 0x9b, 0xde, 0xfb, 0x27, 0xe1, 0x95, 0xd5, 0x14, 0x5b, 0xce, 0x8f,
    0x11, 0xc1, 0x82, 0x18, 0x1e, 0x34, 0x47, 0x58, 0xbd, 0x71, 0x3d, 0xe5,
    0x69, 0xde, 0xeb, 0x2c, 0xe6, 0x5c, 0x46, 0x7b, 0xd1, 0x50, 0x20, 0xe2,
    0x86, 0x76, 0xad, 0x72, 0x4e, 0xa2, 0x3b, 0x53, 0xe6, 0xec, 0xbb, 0x57,
    0xd5, 0xc5, 0x54, 0x0b, 0x58, 0x09, 0xc6, 0xc2, 0xe2, 0xc7, 0x5e, 0x29,
    0x4e, 0xb2, 0x74, 0x9d, 0x87, 0x0a, 0x7c, 0x3a, 0x9e, 0x1e, 0x42, 0x60,
    0x68, 0x62, 0xa3, 0x5d, 0x89, 0x3f, 0xa0, 0xb2, 0xdc, 0x8a, 0x50, 0xd8,
    0x78, 0x2a, 0xb9, 0xe7, 0xb0, 0x91, 0xd7, 0x83, 0x11, 0xb4, 0xac, 0x71,
    0x15, 0x60, 0x3d, 0xcc, 0xf8, 0x2c, 0xb4, 0x80, 0x1d, 0x19, 0xa5, 0x0d,
    0x8f, 0xa1, 0xaf, 0x24, 0x99, 0x9f, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3,
    0x82, 0x01, 0xf1, 0x30, 0x82, 0x01, 0xed, 0x30, 0x1d, 0x06, 0x03, 0x55,
    0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x90, 0x1f, 0x57, 0xaf, 0x17, 0xb5,
    0xec, 0xcf, 0xee, 0x0f, 0xdb, 0x1c, 0x36, 0x9c, 0xa3, 0xe3, 0x15, 0xba,
    0x7a, 0xb5, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff,
    0x04, 0x02, 0x30, 0x00, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04,
    0x18, 0x30, 0x16, 0x80, 0x14, 0x88, 0x27, 0x17, 0x09, 0xa9, 0xb6, 0x18,
    0x60, 0x8b, 0xec, 0xeb, 0xba, 0xf6, 0x47, 0x59, 0xc5, 0x52, 0x54, 0xa3,
    0xb7, 0x30, 0x82, 0x01, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x20, 0x04, 0x82,
    0x01, 0x06, 0x30, 0x82, 0x01, 0x02, 0x30, 0x81, 0xff, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x63, 0x64, 0x05, 0x01, 0x30, 0x81, 0xf1, 0x30,
    0x81, 0xc3, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x02,
    0x30, 0x81, 0xb6, 0x0c, 0x81, 0xb3, 0x52, 0x65, 0x6c, 0x69, 0x61, 0x6e,
    0x63, 0x65, 0x20, 0x6f, 0x6e, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x63,
    0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x62,
    0x79, 0x20, 0x61, 0x6e, 0x79, 0x20, 0x70, 0x61, 0x72, 0x74, 0x79, 0x20,
    0x61, 0x73, 0x73, 0x75, 0x6d, 0x65, 0x73, 0x20, 0x61, 0x63, 0x63, 0x65,
    0x70, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x20, 0x6f, 0x66, 0x20, 0x74, 0x68,
    0x65, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69,
    0x63, 0x61, 0x62, 0x6c, 0x65, 0x20, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x61,
    0x72, 0x64, 0x20, 0x74, 0x65, 0x72, 0x6d, 0x73, 0x20, 0x61, 0x6e, 0x64,
    0x20, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0x20,
    0x6f, 0x66, 0x20, 0x75, 0x73, 0x65, 0x2c, 0x20, 0x63, 0x65, 0x72, 0x74,
    0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x70, 0x6f, 0x6c, 0x69,
    0x63, 0x79, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69,
    0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x70, 0x72, 0x61,
    0x63, 0x74, 0x69, 0x63, 0x65, 0x20, 0x73, 0x74, 0x61, 0x74, 0x65, 0x6d,
    0x65, 0x6e, 0x74, 0x73, 0x2e, 0x30, 0x29, 0x06, 0x08, 0x2b, 0x06, 0x01,
    0x05, 0x05, 0x07, 0x02, 0x01, 0x16, 0x1d, 0x68, 0x74, 0x74, 0x70, 0x3a,
    0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e,
    0x63, 0x6f, 0x6d, 0x2f, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x63, 0x61, 0x2f,
    0x30, 0x4d, 0x06, 0x03, 0x55, 0x1d, 0x1f, 0x04, 0x46, 0x30, 0x44, 0x30,
    0x42, 0xa0, 0x40, 0xa0, 0x3e, 0x86, 0x3c, 0x68, 0x74, 0x74, 0x70, 0x3a,
    0x2f, 0x2f, 0x64, 0x65, 0x76, 0x65, 0x6c, 0x6f, 0x70, 0x65, 0x72, 0x2e,
    0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x63, 0x65,
    0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x61,
    0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x2f, 0x77, 0x77, 0x64,
    0x72, 0x63, 0x61, 0x2e, 0x63, 0x72, 0x6c, 0x30, 0x0e, 0x06, 0x03, 0x55,
    0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x07, 0x80, 0x30,
    0x16, 0x06, 0x03, 0x55, 0x1d, 0x25, 0x01, 0x01, 0xff, 0x04, 0x0c, 0x30,
    0x0a, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x03, 0x30,
    0x13, 0x06, 0x0a, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x63, 0x64, 0x06, 0x01,
    0x02, 0x01, 0x01, 0xff, 0x04, 0x02, 0x05, 0x00, 0x30, 0x0d, 0x06, 0x09,
    0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03,
    0x82, 0x01, 0x01, 0x00, 0x9e, 0x2c, 0xba, 0x8f, 0x18, 0xe6, 0x04, 0xa8,
    0x03, 0x35, 0xf7, 0x56, 0x24, 0xb9, 0xfc, 0x29, 0x22, 0x86, 0x42, 0xf1,
    0x81, 0xce, 0x82, 0x9a, 0x19, 0x4c, 0x0d, 0xb0, 0x4e, 0xe6, 0x7c, 0xd1,
    0x06, 0xab, 0xef, 0xd3, 0x5b, 0x17, 0xdf, 0xad, 0x84, 0xa2, 0x93, 0x16,
    0x7a, 0x51, 0xd6, 0x94, 0x4a, 0xbb, 0x7b, 0xfa, 0x97, 0x66, 0x52, 0x03,
    0x69, 0x13, 0x14, 0x8a, 0x5c, 0x6c, 0x88, 0x45, 0x0f, 0x69, 0xd0, 0x62,
    0x29, 0x9c, 0xc1, 0xf1, 0x72, 0x95, 0xd7, 0x48, 0x28, 0x6e, 0xe5, 0xa3,
    0x20, 0x68, 0xba, 0xde, 0xe8, 0x88, 0x93, 0xfa, 0x20, 0x05, 0x1d, 0xee,
    0x0e, 0xf5, 0x3f, 0x82, 0xf2, 0xc8, 0xe6, 0xc0, 0x96, 0x50, 0xba, 0x3e,
    0x9e, 0x9d, 0x14, 0x4d, 0xd0, 0x73, 0x7c, 0xdd, 0x56, 0xf6, 0x49, 0x9c,
    0xca, 0xb1, 0x98, 0x1e, 0x5f, 0xee, 0xe3, 0xcb, 0xbb, 0xf7, 0x9d, 0x48,
    0x30, 0xe4, 0x6d, 0x6f, 0xfc, 0xe8, 0xbd, 0x28, 0xb6, 0x25, 0x09, 0x41,
    0x7f, 0xd6, 0xb1, 0x7b, 0x97, 0xab, 0xc3, 0xaf, 0x66, 0xaa, 0x92, 0x17,
    0x30, 0x13, 0x70, 0xb2, 0x23, 0x1a, 0xd8, 0x59, 0x6e, 0xe2, 0x12, 0x5d,
    0x6a, 0x56, 0x1f, 0x56, 0xf0, 0x9d, 0x94, 0x77, 0x75, 0x21, 0x9a, 0x2f,
    0x7d, 0x3c, 0x01, 0x92, 0x56, 0x1c, 0xf4, 0x72, 0x23, 0x94, 0xa4, 0x07,
    0x4c, 0xd5, 0x6f, 0x71, 0x10, 0x1b, 0x7e, 0x98, 0xc2, 0xbb, 0x73, 0xbf,
    0xc9, 0x4a, 0x42, 0x53, 0x17, 0x58, 0x09, 0x30, 0xba, 0x04, 0xf8, 0x97,
    0x3e, 0x13, 0x4e, 0x67, 0x2d, 0xec, 0x73, 0x67, 0x10, 0x28, 0x5f, 0xd4,
    0xda, 0xa5, 0x83, 0xbe, 0x06, 0x83, 0x07, 0xce, 0x59, 0x9d, 0x1e, 0x35,
    0x1d, 0x66, 0x87, 0xae, 0x7a, 0x65, 0xfb, 0x76, 0x9b, 0xcf, 0xd7, 0xf1,
    0xb7, 0x8d, 0x2f, 0x5c, 0x8e, 0xa7, 0xcc, 0x8d
};

static void test_with_cert(const char *name,
                           const UInt8 *cert0_bytes, CFIndex cert0_len,
                           const UInt8 *cert1_bytes, CFIndex cert1_len,
                           SecTrustResultType expectedTrustResult) {
    CFDateRef date=NULL;
    CFArrayRef policies=NULL;
    SecPolicyRef policy=NULL;
    SecTrustRef trust=NULL;
    SecCertificateRef cert0=NULL, cert1=NULL;
    CFMutableArrayRef certs=NULL;
    SecTrustResultType trustResult;
    CFIndex chainLen;

    isnt(cert0 = SecCertificateCreateWithBytes(NULL, cert0_bytes, cert0_len),
         NULL, "%s: create cert0", name);
    isnt(cert1 = SecCertificateCreateWithBytes(NULL, cert1_bytes, cert1_len),
         NULL, "%s: create cert1", name);
    // these chain to the Apple Root CA so it is not provided

    if (!cert0 || !cert1)
        return;

    isnt(certs = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks), NULL, "%s: create cert array", name);
    CFArrayAppendValue(certs, cert0);
    CFArrayAppendValue(certs, cert1);

    isnt(policy = SecPolicyCreateAppleTVOSApplicationSigning(), NULL, "%s: create policy", name);
    policies = CFArrayCreate(NULL, (const void **)&policy, 1, &kCFTypeArrayCallBacks);
    CFRelease(policy);
    policy = NULL;
    ok_status(SecTrustCreateWithCertificates(certs, policies, &trust), "%s: create trust", name);
    CFRelease(policies);
    policies = NULL;
    isnt(date = CFDateCreateForGregorianZuluMoment(NULL, 2014, 7, 20, 12, 0, 0),
         NULL, "%s: create verify date", name);
    ok_status(SecTrustSetVerifyDate(trust, date), "%s: set date", name);
    CFReleaseSafe(date);
    date = NULL;
    ok_status(SecTrustEvaluate(trust, &trustResult), "%s: evaluate trust", name);
    ok(trustResult == expectedTrustResult, "%s: trustResult unexpected (expected %d, got %d)",
       name, (int)expectedTrustResult, (int)trustResult);
    chainLen = SecTrustGetCertificateCount(trust);
    ok(chainLen == 3, "%s: chain length 3 expected (got %d)", name, (int)chainLen);
    CFRelease(trust);
    trust = NULL;

    CFReleaseSafe(cert0);
    CFReleaseSafe(cert1);
    CFReleaseSafe(certs);
}

static void tests(void)
{
    // Those are the certificates which should officially pass.
    test_with_cert("AppleTV Prod",
                   kTestAppleTVOSAppSigLeaf, sizeof(kTestAppleTVOSAppSigLeaf),
                   kTestAppleWWDRIntm, sizeof(kTestAppleWWDRIntm),
                   kSecTrustResultUnspecified);
    test_with_cert("AppleTV Test",
                   kTestAppleTVOSAppSigTestLeaf, sizeof(kTestAppleTVOSAppSigTestLeaf),
                   kTestAppleWWDRIntm, sizeof(kTestAppleWWDRIntm),
                   kSecTrustResultUnspecified);

    // An iPhone dev cert, which has a common intermediate, but not the right marker OIDs, should of course fail.
    test_with_cert("iPhone Dev",
                   kTestiPhoneDevCert, sizeof(kTestiPhoneDevCert),
                   kTestAppleWWDRIntm, sizeof(kTestAppleWWDRIntm),
                   kSecTrustResultRecoverableTrustFailure);

}

int si_81_sectrust_appletv(int argc, char *const *argv)
{
    plan_tests(30);
    
    tests();
    
    return 0;
}
