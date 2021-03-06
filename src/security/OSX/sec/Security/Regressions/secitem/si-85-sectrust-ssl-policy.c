/*
 * Copyright (c) 2015 Apple Inc. All Rights Reserved.
 */

#include <Security/SecPolicyPriv.h>
#include <Security/SecInternal.h>
#include <Security/SecTrust.h>
#include <Security/SecTrustPriv.h>
#include <Security/SecCertificatePriv.h>
#include <AssertMacros.h>
#include <utilities/SecCFWrappers.h>

#include "Security_regressions.h"

unsigned char SSLTrustPolicyTestRootCertificate_cer[987] = {
    0x30, 0x82, 0x03, 0xd7, 0x30, 0x82, 0x02, 0xbf, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x01, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x81, 0x8e, 0x31, 0x21,
    0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x18, 0x53, 0x53, 0x4c,
    0x20, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x50, 0x6f, 0x6c, 0x69, 0x63,
    0x79, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x43, 0x41, 0x31, 0x14, 0x30,
    0x12, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x0b, 0x41, 0x70, 0x70, 0x6c,
    0x65, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x1d, 0x30, 0x1b, 0x06,
    0x03, 0x55, 0x04, 0x0b, 0x0c, 0x14, 0x53, 0x65, 0x63, 0x75, 0x72, 0x69,
    0x74, 0x79, 0x20, 0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x65, 0x72, 0x69,
    0x6e, 0x67, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31,
    0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53,
    0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09, 0x43,
    0x75, 0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f, 0x30, 0x1e, 0x17, 0x0d,
    0x31, 0x35, 0x30, 0x38, 0x32, 0x30, 0x30, 0x32, 0x30, 0x31, 0x31, 0x39,
    0x5a, 0x17, 0x0d, 0x32, 0x35, 0x30, 0x38, 0x31, 0x37, 0x30, 0x32, 0x30,
    0x31, 0x31, 0x39, 0x5a, 0x30, 0x81, 0x8e, 0x31, 0x21, 0x30, 0x1f, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x18, 0x53, 0x53, 0x4c, 0x20, 0x54, 0x72,
    0x75, 0x73, 0x74, 0x20, 0x50, 0x6f, 0x6c, 0x69, 0x63, 0x79, 0x20, 0x54,
    0x65, 0x73, 0x74, 0x20, 0x43, 0x41, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0b, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x2c, 0x20,
    0x49, 0x6e, 0x63, 0x2e, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x03, 0x55, 0x04,
    0x0b, 0x0c, 0x14, 0x53, 0x65, 0x63, 0x75, 0x72, 0x69, 0x74, 0x79, 0x20,
    0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x65, 0x72, 0x69, 0x6e, 0x67, 0x31,
    0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61,
    0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31, 0x0b, 0x30, 0x09,
    0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x12, 0x30,
    0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75, 0x70, 0x65,
    0x72, 0x74, 0x69, 0x6e, 0x6f, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06,
    0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00,
    0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01,
    0x01, 0x00, 0xdf, 0x3b, 0x3f, 0xbc, 0xee, 0xbe, 0x24, 0xf1, 0x44, 0x79,
    0x8b, 0x39, 0x01, 0x3d, 0xd3, 0x9c, 0xe4, 0xf3, 0xd1, 0x2b, 0x43, 0x83,
    0xfe, 0x44, 0xf3, 0xf8, 0xd8, 0xf3, 0xd7, 0xa3, 0xe8, 0x7d, 0xd0, 0x1b,
    0x18, 0x1d, 0x0d, 0x7e, 0x5d, 0x89, 0xb3, 0xc7, 0xe1, 0x3e, 0xbe, 0x6e,
    0xe3, 0xdd, 0x9f, 0x8d, 0x42, 0xd6, 0xb1, 0xd2, 0x63, 0x69, 0x4d, 0x09,
    0xe9, 0x09, 0x21, 0x11, 0x42, 0x1e, 0x78, 0xf7, 0x20, 0x8c, 0x55, 0xf3,
    0x32, 0xeb, 0xd4, 0xed, 0xfd, 0xbd, 0xa7, 0x25, 0x90, 0x0b, 0x24, 0x6a,
    0x86, 0xc1, 0x3f, 0xbc, 0x19, 0xc5, 0x3d, 0x02, 0x52, 0x10, 0xfe, 0xf3,
    0xd3, 0xac, 0x97, 0x2d, 0xf5, 0xa2, 0xf5, 0x92, 0x47, 0xcc, 0x2e, 0x78,
    0x21, 0x6c, 0x57, 0xc8, 0x8d, 0x9e, 0x04, 0x59, 0x83, 0x17, 0xd8, 0x63,
    0x5e, 0xdf, 0xe5, 0x24, 0x3b, 0x34, 0x0b, 0x15, 0x73, 0xec, 0x50, 0x61,
    0x92, 0xef, 0xab, 0x1c, 0xeb, 0x42, 0xdf, 0x76, 0x6b, 0x5f, 0x64, 0xd1,
    0x38, 0xdc, 0xe9, 0x36, 0x82, 0x6a, 0xb3, 0xcc, 0x6f, 0x4a, 0x3b, 0xaf,
    0xd3, 0xf2, 0x1d, 0xf3, 0xf4, 0xd8, 0x0f, 0xa0, 0x5d, 0xf5, 0xdd, 0x21,
    0x92, 0x1f, 0xf1, 0x98, 0x0d, 0x12, 0x72, 0x82, 0x3e, 0xea, 0xc9, 0xf4,
    0x4c, 0x0c, 0x43, 0x3f, 0x1d, 0x18, 0x8a, 0xe5, 0x4d, 0xbd, 0x9f, 0x5b,
    0x11, 0x37, 0xd1, 0x3c, 0xad, 0xdb, 0x72, 0xac, 0x90, 0xd0, 0x72, 0x42,
    0x12, 0xb6, 0xe1, 0x6f, 0x10, 0x77, 0x1e, 0x60, 0x3b, 0x42, 0x31, 0xdc,
    0x9c, 0xdd, 0xfb, 0x36, 0xab, 0x5e, 0x65, 0xf4, 0xab, 0x1c, 0x0d, 0x7f,
    0x1b, 0xff, 0xb0, 0xfa, 0x42, 0x0a, 0x82, 0x2e, 0x43, 0x4c, 0x29, 0x72,
    0x82, 0xcb, 0x61, 0xf4, 0xbf, 0xbb, 0x34, 0x9e, 0x43, 0xac, 0xef, 0x50,
    0xc5, 0xc4, 0x58, 0x7f, 0x65, 0x39, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3,
    0x3e, 0x30, 0x3c, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01,
    0xff, 0x04, 0x08, 0x30, 0x06, 0x01, 0x01, 0xff, 0x02, 0x01, 0x00, 0x30,
    0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03,
    0x02, 0x02, 0x84, 0x30, 0x16, 0x06, 0x03, 0x55, 0x1d, 0x25, 0x01, 0x01,
    0xff, 0x04, 0x0c, 0x30, 0x0a, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05,
    0x07, 0x03, 0x01, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x0d,
    0x4e, 0xac, 0x67, 0xc0, 0xe0, 0xfa, 0x66, 0x2e, 0xc3, 0x3e, 0x90, 0x34,
    0x4e, 0xdf, 0x64, 0xfa, 0x06, 0x2e, 0x4e, 0x99, 0xa0, 0x91, 0xf6, 0x96,
    0xe1, 0x41, 0x5d, 0xf6, 0x23, 0x56, 0xbc, 0x64, 0xf9, 0x79, 0xcd, 0x70,
    0xcb, 0xd0, 0xa3, 0x17, 0x87, 0x64, 0x6c, 0x3d, 0x72, 0xca, 0x4b, 0x5e,
    0xae, 0xbf, 0xef, 0x84, 0x04, 0xd4, 0x30, 0xf3, 0xff, 0xbc, 0xa1, 0x28,
    0x2e, 0xd1, 0xfd, 0x43, 0xa3, 0x18, 0xce, 0x89, 0x00, 0x59, 0x3e, 0x6a,
    0x70, 0x73, 0xca, 0x3c, 0x3c, 0xac, 0x6a, 0xfc, 0xb9, 0x5c, 0x79, 0x14,
    0xd7, 0xc8, 0x19, 0x63, 0x6d, 0x37, 0x28, 0xf9, 0x78, 0xd6, 0xb9, 0x2e,
    0xb2, 0x75, 0xd1, 0x05, 0x9c, 0xce, 0xd4, 0x87, 0xe0, 0x92, 0xf7, 0x46,
    0xdf, 0x73, 0x5f, 0x56, 0x1c, 0xff, 0x95, 0x04, 0xa8, 0xb3, 0xa9, 0x4c,
    0x74, 0x07, 0xc6, 0x0a, 0xb9, 0xcd, 0x4c, 0x17, 0x1f, 0x40, 0x73, 0x7d,
    0xb6, 0x73, 0xc7, 0x28, 0x1f, 0x7d, 0x47, 0x86, 0x2a, 0xa2, 0xa1, 0x83,
    0x8b, 0xa4, 0x46, 0x85, 0xeb, 0x19, 0x8c, 0x5e, 0x3c, 0xa4, 0x73, 0x9d,
    0x04, 0x82, 0xe7, 0x0e, 0x2a, 0x3c, 0x83, 0xa1, 0x10, 0xcc, 0x27, 0x81,
    0x1d, 0x3e, 0x1a, 0x7d, 0x1c, 0x4b, 0xfd, 0x45, 0x39, 0xbb, 0x1a, 0xc5,
    0xae, 0x29, 0x22, 0x56, 0x2c, 0x2a, 0x76, 0xc8, 0x26, 0x9f, 0xf0, 0x4f,
    0x48, 0xc8, 0x9d, 0x20, 0xc9, 0x9d, 0x63, 0xc4, 0xe1, 0xad, 0x70, 0xa9,
    0x75, 0xb3, 0xb2, 0xff, 0x35, 0xeb, 0x89, 0x6a, 0x80, 0x11, 0x60, 0x7d,
    0xab, 0xd5, 0xd2, 0xa4, 0xd3, 0x1c, 0x34, 0x21, 0xdf, 0xbe, 0x0a, 0x4f,
    0xcc, 0x79, 0xca, 0x88, 0x81, 0x2b, 0x06, 0x11, 0x1f, 0x31, 0x22, 0x43,
    0x93, 0x76, 0x2c, 0x90, 0x5b, 0x5f, 0x42, 0x3e, 0x97, 0x61, 0x4b, 0xcc,
    0x22, 0x6e, 0xf0
};

static void runTestForDictionary (const void *test_key, const void *test_value, void *context) {
    CFDictionaryRef test_info = test_value;
    CFStringRef test_name = test_key, file = NULL, reason = NULL, expectedResult = NULL, failReason = NULL;
    CFURLRef cert_file_url = NULL;
    CFDataRef cert_data = NULL;
    bool expectTrustSuccess = false;

    SecCertificateRef leaf = NULL, root = NULL;
    CFStringRef hostname = NULL;
    SecPolicyRef policy = NULL;
    SecTrustRef trust = NULL;
    CFArrayRef anchor_array = NULL;
    CFDateRef date = NULL;

    /* Note that this is built without many of the test convenience macros
     * in order to ensure there's only one "test" per test.
     */

    /* get filename in test dictionary */
    file = CFDictionaryGetValue(test_info, CFSTR("Filename"));
    require_action_quiet(file, cleanup, fail("%@: Unable to load filename from plist", test_name));

    /* get leaf certificate from file */
    cert_file_url = CFBundleCopyResourceURL(CFBundleGetMainBundle(), file, CFSTR("cer"), CFSTR("ssl-policy-certs"));
    require_action_quiet(cert_file_url, cleanup, fail("%@: Unable to get url for cert file %@",
                                                      test_name, file));
    
    SInt32 errorCode;
    require_action_quiet(CFURLCreateDataAndPropertiesFromResource(NULL, cert_file_url, &cert_data, NULL, NULL, &errorCode),
                         cleanup,
                         fail("%@: Could not create cert data for %@ with error %d",
                              test_name, file, (int)errorCode));

    /* create certificates */
    leaf = SecCertificateCreateWithData(NULL, cert_data);
    root = SecCertificateCreateWithBytes(NULL, SSLTrustPolicyTestRootCertificate_cer, sizeof(SSLTrustPolicyTestRootCertificate_cer));
    CFRelease(cert_data);
    require_action_quiet(leaf && root, cleanup, fail("%@: Unable to create certificates", test_name));

    /* create policy */
    hostname = CFDictionaryGetValue(test_info, CFSTR("Hostname"));
    require_action_quiet(hostname, cleanup, fail("%@: Unable to load hostname from plist", test_name));

    policy = SecPolicyCreateSSL(true, hostname);
    require_action_quiet(policy, cleanup, fail("%@: Unable to create SSL policy with hostname %@",
                                               test_name, hostname));

    /* create trust ref */
    OSStatus err = SecTrustCreateWithCertificates(leaf, policy, &trust);
    CFRelease(policy);
    require_noerr_action(err, cleanup, ok_status(err, "SecTrustCreateWithCertificates"));

    /* set anchor in trust ref */
    anchor_array = CFArrayCreate(NULL, (const void **)&root, 1, &kCFTypeArrayCallBacks);
    require_action_quiet(anchor_array, cleanup, fail("%@: Unable to create anchor array", test_name));
    err = SecTrustSetAnchorCertificates(trust, anchor_array);
    require_noerr_action(err, cleanup, ok_status(err, "SecTrustSetAnchorCertificates"));

    /* set date in trust ref */
    date = CFDateCreateForGregorianZuluMoment(NULL, 2015, 8, 24, 12, 0, 0);
    require_action_quiet(date, cleanup, fail("%@: Unable to create very date", test_name));
    err = SecTrustSetVerifyDate(trust, date);
    CFRelease(date);
    require_noerr_action(err, cleanup, ok_status(err, "SecTrustSetVerifyDate"));

    /* evaluate */
    SecTrustResultType actualResult = 0;
    err = SecTrustEvaluate(trust, &actualResult);
    require_noerr_action(err, cleanup, ok_status(err, "SecTrustEvaluate"));
    bool is_valid = (actualResult == kSecTrustResultProceed || actualResult == kSecTrustResultUnspecified);
    if (!is_valid) failReason = SecTrustCopyFailureDescription(trust);
    
    /* get expected result for test */
    expectedResult = CFDictionaryGetValue(test_info, CFSTR("Result"));
    require_action_quiet(expectedResult, cleanup, fail("%@: Unable to get expected result",test_name));
    if (!CFStringCompare(expectedResult, CFSTR("kSecTrustResultUnspecified"), 0) ||
        !CFStringCompare(expectedResult, CFSTR("kSecTrustResultProceed"), 0)) {
        expectTrustSuccess = true;
    }

    /* process results */
    if(!CFDictionaryGetValueIfPresent(test_info, CFSTR("Reason"), (const void **)&reason)) {
        /* not a known failure */
        ok(is_valid == expectTrustSuccess, "%s %@%@",
           expectTrustSuccess ? "REGRESSION" : "SECURITY",
           test_name,
           failReason ? failReason : CFSTR(""));
    }
    else if(reason) {
        /* known failure */
        todo(CFStringGetCStringPtr(reason, kCFStringEncodingUTF8));
        ok(is_valid == expectTrustSuccess, "%@%@",
           test_name, expectTrustSuccess ? (failReason ? failReason : CFSTR("")) : CFSTR(" valid"));
    }
    else {
        fail("%@: unable to get reason for known failure", test_name);
    }

cleanup:
    CFReleaseNull(cert_file_url);
    CFReleaseNull(leaf);
    CFReleaseNull(root);
    CFReleaseNull(trust);
    CFReleaseNull(anchor_array);
    CFReleaseNull(failReason);
}

static void tests(void)
{
    CFDataRef plist_data = NULL;
    CFArrayRef plist = NULL;
    CFPropertyListRef tests_dictionary = NULL;

    plist = CFBundleCopyResourceURLsOfType(CFBundleGetMainBundle(), CFSTR("plist"), CFSTR("ssl-policy-certs"));
    if (CFArrayGetCount(plist) != 1) {
        fail("Incorrect number of plists found in ssl-policy-certs");
        goto exit;
    }

    SInt32 errorCode;
    if(!CFURLCreateDataAndPropertiesFromResource(NULL, CFArrayGetValueAtIndex(plist, 0), &plist_data, NULL, NULL, &errorCode)) {
        fail("Could not create data from plist with error %d", (int)errorCode);
        goto exit;
    }

    CFErrorRef err;
    tests_dictionary = CFPropertyListCreateWithData(NULL, plist_data, kCFPropertyListImmutable, NULL, &err);
    if(!tests_dictionary || (CFGetTypeID(tests_dictionary) != CFDictionaryGetTypeID())) {
        fail("Failed to create tests dictionary from plist");
        goto exit;
    }

    CFDictionaryApplyFunction(tests_dictionary, runTestForDictionary, NULL);

exit:
    CFReleaseNull(plist);
    CFReleaseNull(plist_data);
    CFReleaseNull(tests_dictionary);
}

int si_85_sectrust_ssl_policy(int argc, char *const *argv)
{
    plan_tests(26);

    tests();

    return 0;
}
