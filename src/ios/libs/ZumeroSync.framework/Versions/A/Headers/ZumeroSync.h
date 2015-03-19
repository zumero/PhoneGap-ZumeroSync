//
//  ZumeroSync.h
//
//  ZumeroSync - a simple Objective-C wrapper around the Zumero Client API
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

#import "ZumeroUtil.h"

#include <sqlite3.h>
#include "zumero_client_api.h"

/** provides static methods implementing the Zumero Sync and Quarantine APIs.
 */
@interface ZumeroSync : NSObject

/** @name Sync
 */

/** Sync the local database with the server database.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)	
  @param serverUrl Zumero server url
  @param remote Dbfile name on server
  @param authSchemeJS Scheme part of auth credentials, as JSON (or nil) 
  @param user Username part of auth credentials (or nil)		 
  @param password Password part of auth credentials (or nil)	 
  @param error On failure, localizedDescription will contain error text		

  @return `YES` if sync was successful
 */
+ (BOOL) Sync:(NSString *)filename
	cipherKey:(NSString *)cipherKey
	serverUrl:(NSString *)serverUrl
	   remote:(NSString *)remote
 authSchemeJS:(NSString *)authSchemeJS
		 user:(NSString *)user
	 password:(NSString *)password
		error:(NSError **)error;


/** Sync the local database with the server database.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)	
  @param serverUrl Zumero server url
  @param remote Dbfile name on server
  @param authScheme Scheme part of auth credentials (or nil) 
  @param user Username part of auth credentials (or nil)		 
  @param password Password part of auth credentials (or nil)	 
  @param error On failure, localizedDescription will contain error text		

  @return `YES` if sync was successful
 */
+ (BOOL) Sync:(NSString *)filename
	cipherKey:(NSString *)cipherKey
	serverUrl:(NSString *)serverUrl
	   remote:(NSString *)remote
   authScheme:(NSDictionary *)authScheme
		 user:(NSString *)user
	 password:(NSString *)password
		error:(NSError **)error;


/** @name Quarantine
 */

/**  Move un-synced local changes into an isolated holding area. Typically, the
  reason to do so is because the local changes conflict with other changes
  already on the server.
 
  The quarantined changes are stored locally, so no network activity is
  required.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)	
  @param pqid OUT: quarantine id written here
  @param error On failure, localizedDescription will contain error text

  @return `YES` if quarantine was successful
 */
+ (BOOL) QuarantineSinceLastSync:(NSString *)filename
				       cipherKey:(NSString *)cipherKey
			  			    pqid:(sqlite3_int64 *)pqid
						   error:(NSError **)error;

// older, incorrect name, left here for compatibility
+ (BOOL) QuarantineSyncLastSync:(NSString *)filename
					  cipherKey:(NSString *)cipherKey
						   pqid:(sqlite3_int64 *)pqid
						  error:(NSError **)error;

/** Sync the local database with the server database, including the changes
  stored in the specified quarantine.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)	
  @param qid quarantine id from which to sync
  @param serverUrl Zumero server url
  @param remote Dbfile name on server
  @param authScheme Scheme part of auth credentials (or nil) 
  @param user Username part of auth credentials (or nil)		 
  @param password Password part of auth credentials (or nil)	 
  @param partial OUT: YES if SyncQuarantine must be called again
  @param error On failure, localizedDescription will contain error text		

  @return `YES` if sync was successful
 */ 
+ (BOOL) SyncQuarantine:(NSString *)filename
			  cipherKey:(NSString *)cipherKey
					qid:(sqlite3_int64)qid
			  serverUrl:(NSString *)serverUrl
				 remote:(NSString *)remote
			 authScheme:(NSDictionary *)authScheme
				   user:(NSString *)user
			   password:(NSString *)password
				partial:(BOOL *)partial
				  error:(NSError **)error;

/** Sync the local database with the server database, including the changes
  stored in the specified quarantine.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)	
  @param qid quarantine id from which to sync
  @param serverUrl Zumero server url
  @param remote Dbfile name on server
  @param authSchemeJS Scheme part of auth credentials, as JSON (or nil) 
  @param user Username part of auth credentials (or nil)		 
  @param password Password part of auth credentials (or nil)	 
  @param partial OUT: YES if SyncQuarantine must be called again
  @param error On failure, localizedDescription will contain error text		

  @return `YES` if sync was successful
 */ 
+ (BOOL) SyncQuarantine:(NSString *)filename
			  cipherKey:(NSString *)cipherKey
					qid:(sqlite3_int64)qid
			  serverUrl:(NSString *)serverUrl
				 remote:(NSString *)remote
		   authSchemeJS:(NSString *)authSchemeJS
				   user:(NSString *)user
			   password:(NSString *)password
				partial:(BOOL *)partial
				  error:(NSError **)error;


/**  Permanently delete quarantined changes.

  @param filename Database filename (UTF-8)
  @param cipherKey Key to unlock encrypted database (or nil)
  @param qid Quarantine id
  @param error On failure, localizedDescription will contain error text

  @return `YES` if the changes were deleted
 */
+ (BOOL) DeleteQuarantine:(NSString *)filename
				cipherKey:(NSString *)cipherKey
					  qid:(sqlite3_int64)qid
					error:(NSError **)error;

@end
