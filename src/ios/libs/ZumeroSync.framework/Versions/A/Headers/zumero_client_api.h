/*
 *   Copyright 2013-2015 Zumero LLC
 * 
 *   Licensed under the Zumero Client SDK License (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 * 
 *      http://zumero.com/dev-center/zss/sdk-license/
 * 
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#ifndef _ZUMERO_CLIENT_API_H_
#define _ZUMERO_CLIENT_API_H_

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

/*
** Result Codes
**
** Most Zumero functions return an integer result code to indicate success or
** failure. In some cases, that result code may be a SQLite result code.
** Or, it may be a Zumero-specific result code defined below.
**
** The zumero_errstr() function can be used to obtain an English-language
** string corresponding to any Zumero-specific result code.
**
** WARNING: If you change the numeric value of any result code, you must also
**          change the desktop/Xamarin wrapper to match:
**              @/src/sdk/netfx/zumero.cs
**          If you add or remove result codes, then you'll need to change
**          that wrapper as well as the WinRT/WP8 wrappers:
**              @/src/sdk/windows/shared/client_component/ZumeroClient.h
**              @/src/sdk/windows/shared/client_netfx/ZumeroClient.cs
*/

#define ZUMERO_ERROR    200              /* low byte of any result code which is an error */
#define ZUMERO_PARTIAL  201              /* not an error */

#define ZUMERO_NETWORK_CONNECTION_FAILED         (ZUMERO_ERROR | (1<<8))
#define ZUMERO_AUTHENTICATION_FAILED             (ZUMERO_ERROR | (2<<8))
#define ZUMERO_PERMISSION_DENIED                 (ZUMERO_ERROR | (3<<8))
#define ZUMERO_HTTP_400                          (ZUMERO_ERROR | (4<<8))
#define ZUMERO_HTTP_406                          (ZUMERO_ERROR | (5<<8))
#define ZUMERO_HTTP_500                          (ZUMERO_ERROR | (6<<8))
#define ZUMERO_HTTP_404                          (ZUMERO_ERROR | (7<<8))
#define ZUMERO_HTTP_502                          (ZUMERO_ERROR | (8<<8))
#define ZUMERO_HTTP_OTHER                        (ZUMERO_ERROR | (9<<8))
#define ZUMERO_CONSTRAINT_VIOLATION              (ZUMERO_ERROR | (10<<8))
#define ZUMERO_UNIQUE_CONSTRAINT_VIOLATION       (ZUMERO_ERROR | (11<<8))
#define ZUMERO_CHECK_CONSTRAINT_VIOLATION        (ZUMERO_ERROR | (12<<8))
#define ZUMERO_FOREIGN_KEY_CONSTRAINT_VIOLATION  (ZUMERO_ERROR | (13<<8))
#define ZUMERO_SYNC_REJECT_BY_RULE               (ZUMERO_ERROR | (14<<8))
#define ZUMERO_WRONG_DBFILE                      (ZUMERO_ERROR | (15<<8))
#define ZUMERO_SERVER_ROLLBACK_DETECTED          (ZUMERO_ERROR | (16<<8))
#define ZUMERO_DBFILE_NOT_FOUND                  (ZUMERO_ERROR | (17<<8))
#define ZUMERO_TOO_MANY_DEADLOCKS                (ZUMERO_ERROR | (18<<8))
#define ZUMERO_LICENSE_ERROR                     (ZUMERO_ERROR | (19<<8))
#define ZUMERO_CLIENT_CORRUPTION_DETECTED        (ZUMERO_ERROR | (20<<8))
#define ZUMERO_SERVER_MISCONFIGURATION           (ZUMERO_ERROR | (21<<8))
#define ZUMERO_INVALIDARG                        (ZUMERO_ERROR | (30<<8))
#define ZUMERO_INVALID_DBFILE_NAME               (ZUMERO_ERROR | (50<<8))
#define ZUMERO_INVALID_AUTH_SCHEME_STRING        (ZUMERO_ERROR | (51<<8))
#define ZUMERO_UNSUPPORTED                       (ZUMERO_ERROR | (52<<8))
#define ZUMERO_DIFFERENT_CREDENTIALS             (ZUMERO_ERROR | (53<<8))

/*
** Sync the local database with the server database.
*/
int zumero_sync(
  const char *zFilename,     /* Database filename (UTF-8) */
  const char *zCipherKey,    /* Key to unlock encrypted database */
  const char *zServerUrl,    /* Zumero server url */
  const char *zDbfile,       /* Dbfile name on server */
  const char *zAuthScheme,   /* Scheme part of auth credentials */
  const char *zUser,         /* Username part of auth credentials */
  const char *zPassword,     /* Password part of auth credentials */
  const char *zTempDir,      /* Temp directory filename */
  char **pzErrorDetails      /* OUT: Error message written here */
);

/*
** Move un-synced local changes into an isolated holding area. Typically, the
** reason to do so is because the local changes conflict with other changes
** already on the server.
**
** The quarantined changes are stored locally, so no network activity is
** required.
*/
int zumero_quarantine_since_last_sync(
  const char *zFilename,     /* Database filename (UTF-8) */
  const char *zCipherKey,    /* Key to unlock encrypted database */
  const char *zTempDir,      /* Temp directory filename */
  sqlite3_int64* pnQid,      /* OUT: quarantine id written here */
  char **pzErrorDetails      /* OUT: Error message written here */
);

/*
** Sync the local database with the server database, including the changes
** stored in the specified quarantine.
*/
int zumero_sync_quarantine(
  const char *zFilename,     /* Database filename (UTF-8) */
  const char *zCipherKey,    /* Key to unlock encrypted database */
  sqlite3_int64 qid,         /* Quarantine id */
  const char *zServerUrl,    /* Zumero server url */
  const char *zDbfile,       /* Dbfile name on server */
  const char *zAuthScheme,   /* Scheme part of auth credentials */
  const char *zUser,         /* Username part of auth credentials */
  const char *zPassword,     /* Password part of auth credentials */
  const char *zTempDir,      /* Temp directory filename */
  char **pzErrorDetails      /* OUT: Error message written here */
);

/*
** Permanently delete quarantined changes.
*/
int zumero_delete_quarantine(
  const char *zFilename,     /* Database filename (UTF-8) */
  const char *zCipherKey,    /* Key to unlock encrypted database */
  sqlite3_int64 qid,         /* Quarantine id */
  char **pzErrorDetails      /* OUT: Error message written here */
);

/*
** Free memory stored in a pointer returned from a Zumero function.
*/
void zumero_free(void*);

/*
** Return the English-language text that describes the result code.
*/
const char* zumero_errstr(int);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
#endif
