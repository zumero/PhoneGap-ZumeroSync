//
//  ZumeroUtil.h
//  ZumeroSync
//
//  Copyright 2013-2015 Zumero LLC
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#import <Foundation/Foundation.h>

/** Miscellaneous methods to manage Zumero's representation of MS SQL data.
 */
@interface ZumeroUtil : NSObject

/** given a 16-byte blob (in NSData form), return an MSSQL-style
    GUID string ("aaba1234-5678-9090-9876-aabbccdd1234")

	@param uuid the blob to be encoded 
	@return an MSSQL string representation of `uuid`
 */
+ (NSString *) dataToSqlGuid:(NSData *)uuid;

/** given an MSSQL-style GUID string ("aaba1234-5678-9090-9876-aabbccdd1234"),
    return the corresponding 16-byte blob

    @param sqlguid the string to be encoded
    @return the binary representation of `sqlguid`
 */
+ (NSData *)   sqlGuidToData:(NSString *)sqlguid;

@end
