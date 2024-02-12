// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CAST_STANDALONE_RECEIVER_PRIVATE_KEY_DER_H_
#define CAST_STANDALONE_RECEIVER_PRIVATE_KEY_DER_H_

#include <array>

namespace openscreen {
namespace cast {

// Important note about private keys and security: For example usage purposes,
// we have checked in a default private key here. However, in a production
// environment keys should never be checked into source control. This is an
// example self-signed private key for TLS.
//
// Generated using the following command:
// $ xxd -i <path/to/private_key.der>
std::array<uint8_t, 1192> kPrivateKeyDer = {
    0x30, 0x82, 0x04, 0xa4, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00,
    0xb8, 0x07, 0xbb, 0x3f, 0xde, 0x47, 0xba, 0xec, 0x7a, 0xc2, 0x6e, 0xe5,
    0x44, 0x4a, 0xa8, 0x50, 0xd9, 0xef, 0xc1, 0x67, 0xc1, 0x5e, 0x14, 0xc0,
    0x1a, 0x1f, 0x8e, 0x83, 0xb9, 0xb2, 0x5d, 0x2d, 0x74, 0x12, 0x4b, 0x43,
    0x3c, 0xa6, 0xfb, 0xc4, 0x6c, 0xb0, 0xab, 0xda, 0xfb, 0xfd, 0x51, 0x18,
    0xc1, 0xc1, 0x22, 0x05, 0xf5, 0x2b, 0x10, 0xe4, 0x84, 0x1b, 0xa7, 0xdc,
    0xe6, 0xd0, 0xf5, 0x64, 0xa7, 0xc7, 0x6f, 0x1a, 0x85, 0xf8, 0xc4, 0xfb,
    0xbf, 0x17, 0x54, 0xaa, 0x11, 0x29, 0xfe, 0xda, 0x33, 0x9e, 0xfa, 0xb1,
    0x86, 0x9b, 0xf4, 0xcd, 0xf5, 0xe1, 0x3f, 0x7b, 0x3b, 0xad, 0xd9, 0xe2,
    0xc7, 0x6e, 0x4f, 0x1e, 0xa8, 0x13, 0x22, 0xa2, 0x7a, 0xcf, 0xe1, 0x8a,
    0x06, 0xf3, 0x28, 0x3a, 0xdc, 0xd3, 0x8c, 0x24, 0xa6, 0xe0, 0xd3, 0x5a,
    0x23, 0x21, 0x53, 0x02, 0x7d, 0x08, 0x30, 0xcb, 0xf1, 0x21, 0xca, 0x72,
    0x69, 0x49, 0x6e, 0x0f, 0xbc, 0x03, 0x7e, 0x0e, 0x60, 0x5d, 0x92, 0x08,
    0x3f, 0x04, 0x76, 0x62, 0x2d, 0x4b, 0xeb, 0x61, 0xaa, 0xe6, 0xcd, 0x2f,
    0x28, 0x24, 0xb0, 0xe8, 0xa5, 0xfe, 0x89, 0x90, 0xb8, 0xa4, 0x60, 0x6e,
    0x4c, 0x8c, 0x2f, 0xa3, 0xae, 0x72, 0xf2, 0x42, 0xb9, 0xc9, 0xa2, 0x6f,
    0x91, 0xbc, 0x75, 0x3b, 0x35, 0xb7, 0xe6, 0x24, 0xcb, 0x80, 0x8a, 0x34,
    0xfa, 0x9d, 0xf1, 0x7c, 0x88, 0x98, 0x09, 0x7b, 0x50, 0x56, 0xa5, 0x84,
    0x9c, 0x5f, 0x6c, 0x6e, 0x10, 0xfa, 0x95, 0xb6, 0xbf, 0xe4, 0xb0, 0x55,
    0x29, 0x3d, 0xe6, 0x3d, 0x14, 0xd7, 0x70, 0x17, 0xd8, 0xd3, 0xaa, 0xaf,
    0x4f, 0x15, 0x99, 0x63, 0xd0, 0x74, 0xfc, 0xb0, 0x6b, 0x66, 0x28, 0x02,
    0xd1, 0xbb, 0x01, 0x57, 0x02, 0xfe, 0x52, 0xe2, 0x0b, 0xbd, 0x8c, 0x0a,
    0x87, 0x8b, 0x60, 0xe9, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x82, 0x01,
    0x01, 0x00, 0xaa, 0x19, 0x7b, 0x5a, 0x6d, 0x7a, 0x9f, 0xac, 0x35, 0x4b,
    0xc2, 0x74, 0xe7, 0xca, 0x9a, 0x09, 0x21, 0x68, 0x1a, 0xbc, 0x6c, 0x5f,
    0x29, 0x8e, 0xe6, 0x96, 0x84, 0x83, 0xfd, 0x00, 0x80, 0x5f, 0xa3, 0x09,
    0xc5, 0xc7, 0x40, 0x28, 0x98, 0x4d, 0xd6, 0xa8, 0xf6, 0x30, 0x52, 0xfa,
    0xb2, 0x1a, 0xcf, 0xfc, 0x54, 0x16, 0x6d, 0xa6, 0x80, 0xd6, 0xb7, 0xc5,
    0x58, 0x43, 0x36, 0x95, 0xae, 0x3c, 0x7b, 0x58, 0x3b, 0xb9, 0xa8, 0x5b,
    0x68, 0xb7, 0xc8, 0xc9, 0x27, 0xd8, 0x8a, 0x44, 0xe6, 0xeb, 0x89, 0x0b,
    0x49, 0x6d, 0x0d, 0x9e, 0xd9, 0x88, 0x05, 0xdd, 0x4d, 0x6f, 0xfa, 0x99,
    0x96, 0xeb, 0xa6, 0xaa, 0xaf, 0x37, 0x06, 0xe3, 0xa8, 0xff, 0xc5, 0xc4,
    0xa0, 0x13, 0x94, 0x98, 0xec, 0x76, 0x7b, 0xe6, 0x8d, 0x82, 0xd3, 0x3c,
    0xbc, 0x1e, 0x74, 0x9a, 0x38, 0xbf, 0xf4, 0x11, 0xbe, 0x07, 0x32, 0x2d,
    0x16, 0x2c, 0xf2, 0x5d, 0x24, 0x38, 0x70, 0xfb, 0x90, 0x8a, 0x38, 0xd6,
    0x17, 0xe1, 0x66, 0x92, 0x38, 0x06, 0x97, 0xb3, 0x07, 0xfd, 0x77, 0xe2,
    0xe7, 0x49, 0xae, 0x5a, 0xbc, 0xe5, 0xa8, 0xca, 0xe1, 0x0f, 0xb6, 0x4c,
    0x05, 0x73, 0x3f, 0x11, 0xd0, 0xf9, 0x1e, 0xba, 0x53, 0x48, 0xf5, 0xaf,
    0x28, 0x5b, 0xea, 0x12, 0x63, 0xbc, 0x84, 0xa7, 0x5f, 0x2e, 0x1d, 0x3e,
    0x02, 0x54, 0x58, 0xed, 0x2b, 0x42, 0xf9, 0xc6, 0x0c, 0xd4, 0x24, 0x77,
    0x1a, 0x2c, 0xbf, 0x75, 0x92, 0xf7, 0xcb, 0xd4, 0x58, 0x2f, 0x88, 0x2d,
    0xe8, 0x16, 0xca, 0xe5, 0x25, 0xe8, 0x5b, 0xbd, 0x53, 0x26, 0x23, 0xe0,
    0xa9, 0x35, 0x4d, 0xdb, 0x51, 0x85, 0x63, 0x20, 0xad, 0x61, 0xd2, 0x6d,
    0xbf, 0x01, 0x7d, 0x04, 0x44, 0x02, 0x96, 0x92, 0x36, 0x19, 0xed, 0xd1,
    0xd8, 0x16, 0x86, 0x06, 0xd4, 0x81, 0x02, 0x81, 0x81, 0x00, 0xe1, 0xa6,
    0xca, 0xb3, 0xef, 0xfe, 0x9f, 0xd6, 0xac, 0x58, 0x5c, 0x17, 0x88, 0xaf,
    0x4d, 0x85, 0x29, 0x50, 0x1f, 0x66, 0x90, 0x9b, 0x81, 0xb6, 0x82, 0x0d,
    0xc3, 0x5a, 0xa8, 0x8a, 0x2b, 0x7f, 0x58, 0x9b, 0x07, 0xe6, 0x64, 0xf7,
    0x1c, 0x77, 0x9d, 0x53, 0x97, 0xa0, 0x33, 0x14, 0x6e, 0x77, 0x1e, 0xe3,
    0x00, 0x0f, 0xb2, 0xb1, 0x69, 0x25, 0x3d, 0x63, 0x3c, 0xe1, 0xbb, 0x41,
    0x74, 0x97, 0x2d, 0x5e, 0x14, 0x79, 0x93, 0x38, 0x15, 0xbe, 0x52, 0x74,
    0x64, 0xc0, 0xfd, 0x22, 0x8e, 0xd7, 0xc9, 0xfb, 0x66, 0x55, 0xce, 0x5b,
    0x6a, 0x6f, 0x00, 0xed, 0x03, 0x7e, 0x4b, 0x9c, 0x4b, 0x8b, 0x3a, 0x50,
    0x65, 0x0d, 0x70, 0x9b, 0xdb, 0xf7, 0x1f, 0xd7, 0x66, 0x7a, 0xd1, 0x1e,
    0xa0, 0x8f, 0xe6, 0x03, 0x12, 0x18, 0x52, 0x25, 0x41, 0xa7, 0xb9, 0x8e,
    0x75, 0x63, 0x11, 0xd2, 0x63, 0xd7, 0x02, 0x81, 0x81, 0x00, 0xd0, 0xc7,
    0xe9, 0x97, 0x38, 0x33, 0x95, 0xbd, 0x18, 0xa5, 0x0a, 0x68, 0xab, 0xba,
    0x5e, 0x3e, 0x1f, 0x16, 0x86, 0xc0, 0x50, 0x09, 0xab, 0x52, 0xb7, 0x62,
    0x4e, 0x34, 0xb1, 0xc1, 0xd3, 0xb5, 0xf4, 0xe0, 0x04, 0x30, 0xa6, 0xdd,
    0x4a, 0xba, 0x7c, 0x59, 0xed, 0xd7, 0x76, 0xd3, 0x02, 0xe7, 0x05, 0x18,
    0x00, 0xdb, 0x65, 0xf2, 0x82, 0xe4, 0xfa, 0xbf, 0x9d, 0xad, 0x1a, 0x56,
    0x7b, 0x5e, 0xef, 0xff, 0x9b, 0xe5, 0x2f, 0x7c, 0xdd, 0x50, 0x53, 0x2b,
    0x6b, 0xc0, 0xac, 0x7b, 0x21, 0x8d, 0xc3, 0x39, 0xfe, 0xd0, 0x1a, 0xed,
    0xd1, 0xb6, 0x56, 0xda, 0x9e, 0x87, 0x9a, 0x6a, 0x69, 0x81, 0x29, 0x81,
    0x75, 0x69, 0xa6, 0x25, 0xc2, 0xf7, 0x5a, 0x94, 0x97, 0x6a, 0x7a, 0xf9,
    0x6c, 0xbe, 0x43, 0x76, 0x34, 0xba, 0x0c, 0x50, 0x6d, 0x22, 0xe8, 0xa6,
    0x9c, 0x80, 0x62, 0x87, 0xc9, 0x3f, 0x02, 0x81, 0x80, 0x78, 0xaf, 0x47,
    0x1c, 0x63, 0x90, 0x30, 0x16, 0x95, 0x88, 0x90, 0x80, 0x79, 0xb7, 0x20,
    0x63, 0xc6, 0xcb, 0xb6, 0x6f, 0x99, 0x89, 0xc2, 0x1f, 0x45, 0x81, 0x6c,
    0xe9, 0x10, 0xd9, 0x0d, 0x18, 0x87, 0xe0, 0x2a, 0xa2, 0x7b, 0x7f, 0x7a,
    0x77, 0x32, 0xea, 0xa1, 0x5e, 0xa9, 0xd3, 0x14, 0x9d, 0x9b, 0x24, 0x57,
    0x45, 0x0e, 0x12, 0x3a, 0xa5, 0x13, 0x26, 0xff, 0x49, 0xcf, 0x67, 0xdb,
    0x9e, 0x7b, 0x42, 0x24, 0xfb, 0x3c, 0xd4, 0xb3, 0x34, 0x5e, 0x4f, 0x28,
    0x0f, 0xdb, 0x92, 0xdf, 0x08, 0xe4, 0x5b, 0x13, 0xc9, 0x72, 0x9b, 0x8b,
    0xda, 0x20, 0x89, 0xa2, 0xe3, 0xaa, 0x36, 0xc6, 0x64, 0x89, 0x64, 0xb4,
    0x17, 0x33, 0x11, 0xf8, 0xdc, 0x3b, 0xe8, 0x6d, 0x43, 0xe4, 0x92, 0x57,
    0xd7, 0x7e, 0x72, 0x47, 0xfc, 0x3f, 0xfa, 0xf3, 0x19, 0x6c, 0x71, 0x97,
    0xb0, 0xcb, 0xb8, 0x55, 0x73, 0x02, 0x81, 0x81, 0x00, 0x98, 0xf1, 0xfa,
    0x73, 0x67, 0x1e, 0x93, 0x11, 0x45, 0xde, 0x91, 0xb3, 0x80, 0x2a, 0x35,
    0x23, 0xf9, 0x0e, 0x3d, 0x84, 0xe0, 0x9d, 0x54, 0xbe, 0x71, 0xcd, 0x38,
    0x51, 0x6d, 0xee, 0xfa, 0x33, 0x0f, 0xc2, 0x94, 0x0f, 0x38, 0x0e, 0x60,
    0xd2, 0x20, 0x8a, 0x98, 0xac, 0x01, 0x46, 0x2f, 0x98, 0x21, 0xa9, 0x25,
    0xe7, 0x93, 0xd5, 0x86, 0x82, 0x4c, 0x16, 0xd7, 0x61, 0x9a, 0x2b, 0xc4,
    0x91, 0x15, 0xec, 0x00, 0xbe, 0x72, 0x7d, 0x5c, 0x7b, 0x9d, 0x91, 0xef,
    0x8b, 0xe4, 0x4f, 0x07, 0x93, 0x9c, 0x72, 0xfd, 0xf2, 0x61, 0xe7, 0xda,
    0x7b, 0x63, 0x41, 0x20, 0x65, 0x62, 0x7f, 0x95, 0xee, 0xa3, 0x03, 0x4d,
    0x8a, 0x29, 0xc6, 0xfb, 0xfe, 0xcc, 0x82, 0x92, 0x31, 0xd5, 0x08, 0xa7,
    0xda, 0xf1, 0xfc, 0xc4, 0x3f, 0x8f, 0x09, 0xd4, 0x09, 0x80, 0xb9, 0x9d,
    0x68, 0x87, 0xc5, 0xc5, 0x6d, 0x02, 0x81, 0x80, 0x1f, 0xd9, 0x20, 0xde,
    0xba, 0xcd, 0x63, 0x34, 0x4f, 0x9f, 0xbb, 0x05, 0x0a, 0x8d, 0x20, 0xe1,
    0x66, 0x41, 0x2f, 0xae, 0xc7, 0xfa, 0x5d, 0xfd, 0xb7, 0x2a, 0x0f, 0xa6,
    0x6d, 0xf3, 0xad, 0x65, 0x54, 0x75, 0x2c, 0x26, 0x1e, 0xac, 0x1f, 0x24,
    0x4c, 0x83, 0xe3, 0x28, 0x08, 0x60, 0x74, 0xfe, 0xa9, 0x53, 0x36, 0x1e,
    0xb3, 0x39, 0x9d, 0xe7, 0x49, 0x03, 0x66, 0x61, 0xe8, 0xd4, 0xf4, 0xd8,
    0x65, 0x57, 0x01, 0xed, 0xaa, 0x7b, 0x6b, 0x04, 0xa2, 0x5f, 0xe1, 0x67,
    0xe6, 0x06, 0x7c, 0x84, 0x2a, 0x7d, 0x53, 0x03, 0x1c, 0x9c, 0x82, 0x08,
    0x37, 0x07, 0xaf, 0x77, 0xe0, 0x99, 0x69, 0xce, 0x01, 0x5a, 0x85, 0x4b,
    0x27, 0xb9, 0xb2, 0x20, 0x8c, 0xa5, 0xb9, 0x42, 0x2f, 0xad, 0x56, 0xdd,
    0xb9, 0x0d, 0x23, 0x05, 0x53, 0x5a, 0x26, 0x3a, 0xe2, 0x17, 0x58, 0x79,
    0x96, 0x8c, 0x5a, 0x05};

}  // namespace cast
}  // namespace openscreen

#endif  // CAST_STANDALONE_RECEIVER_PRIVATE_KEY_DER_H_