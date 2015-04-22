# Zumero PhoneGap Plugin for iOS and Android

This plugin allows applications built on PhoneGap
to synchronize with [Zumero for SQL Server (ZSS)][zss].


## Zumero objects

### Zumero

You'll need a single `Zumero` object, created by requiring the 
Zumero module:

    var zumero = cordova.require("cordova/plugin/zumero");

Methods:

#### sync

    sync( fullPath, 
          encryptionKey, 
          serverURL, 
          dbFileName, 
          scheme, user, password, 
          successCallback, errorCallback)

- `fullPath`: the full name of the local SQLite database
- `encryptionKey`: this db's SQLCipher encryption key, or `null`
- `serverUrl`: the URL of your Zumero instance
- `dbFileName`: the name of the remote dbfile
- `scheme`: the authentication scheme (or `null`)
- `user`: the user name (or `null`)
- `password`: the password (or `null`)
- `successCallback`: function, called when the operation
 succeeds
- `errorCallback`: function, called when the operation fails 
 (optional)

### Example

    zumero.sync("/data/data/com.example.myapp/mydb", 
       "",
       "https://zss.example.com", 
       "mydb", 
       '{"scheme_type":"table", "table":"users"}',
       "user", 
       "password", 
       function() {
          // success
       },
       function(result) {
          // failure - result.code and result.message will
          // contain details
       }
    );

#### quarantineSinceLastSync

    quarantineSinceLastSync( fullPath, 
          encryptionKey, 
          successCallback, errorCallback)

- `fullPath`: the full name of the local SQLite database
- `encryptionKey`: this db's SQLCipher encryption key, or `null`
- `successCallback`: function, called when the operation
 succeeds.  The quarantineID of the newly created quarantine 
 package is sent as the only argument to this function.
- `errorCallback`: function, called when the operation fails 
 (optional)

### Example

    zumero.quarantineSinceLastSync("/data/data/com.example.myapp/mydb", 
       "",
       function(quarantineID) {
          // success
       },
       function(result) {
          // failure - result.code and result.message will
          // contain details
       }
    );

#### syncQuarantine

    syncQuarantine( fullPath, 
          encryptionKey, 
          quarantineID, 
          serverURL, 
          dbFileName, 
          scheme, user, password, 
          successCallback, errorCallback)

- `fullPath`: the full name of the local SQLite database
- `encryptionKey`: this db's SQLCipher encryption key, or `null`
- `quarantineID`: the quarantineID that was returned from `quarantineSinceLastSync`
- `serverUrl`: the URL of your Zumero instance
- `dbFileName`: the name of the remote dbfile
- `scheme`: the authentication scheme (or `null`)
- `user`: the user name (or `null`)
- `password`: the password (or `null`)
- `successCallback`: function, called when the operation
 succeeds
- `errorCallback`: function, called when the operation fails 
 (optional)

### Example

    zumero.syncQuarantine("/data/data/com.example.myapp/mydb", 
       "",
       1,
       "https://zss.example.com", 
       "mydb", 
       '{"scheme_type":"table", "table":"users"}',
       "user", 
       "password", 
       function() {
          // success
       },
       function(result) {
          // failure - result.code and result.message will
          // contain details
       }
    );

#### deleteQuarantine

    deleteQuarantine( fullPath, 
          encryptionKey, 
          quarantineID, 
          successCallback, errorCallback)

- `fullPath`: the full name of the local SQLite database
- `encryptionKey`: this db's SQLCipher encryption key, or `null`
- `quarantineID`: the quarantineID that was returned from `quarantineSinceLastSync`
- `successCallback`: function, called when the operation
 succeeds.
- `errorCallback`: function, called when the operation fails 
 (optional)

### Example

    zumero.deleteQuarantine("/data/data/com.example.myapp/mydb", 
       "",
       1,
       function() {
          // success
       },
       function(result) {
          // failure - result.code and result.message will
          // contain details
       }
    );

## References

For more information on authorization schemes, dbfiles, and sync, see
the [Zumero Client API docs][docs].

Zumero needs the full path to the dbfile being synchronized; you'll either
need to know where your SQLite library stores its files (and if it changes
their filenames) and add that information to your Zumero calls, or use a
SQLite library that exposes the full name of an opened dbfile.

A Zumero-modified fork of the lite4cordova SQLite plugin can be found at
[github.com/zumero/Cordova-SQLitePlugin][splugin] (and is required by this plugin) - it extends the `openDatabase`
method's "success" handler function, adding a "full path" parameter after the
"message" parameter.  This is the path you'd use when synching.

    var db = window.sqlitePlugin.openDatabase( {name: dbName},
        function(msg, fullname) {
            // save for later
            myDbFilename = fullname;

            // sync right now
            zumero.sync(fullname, 
               "https://zss.example.com", 
               dbName, 
               '{"scheme_type":"table", "table":"users"}',
               "syncuser", 
               "syncpassword", 
               function() {
                  // success
               },
               null
            );
        }
    );

[zss]: http://zumero.com/
[docs]: http://zumero.com/docs/zumero_for_sql_server_client_api.html
[splugin]: https://github.com/zumero/Cordova-SQLitePlugin
