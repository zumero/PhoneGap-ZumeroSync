# Zumero PhoneGap Plugins for iOS and Android

The plugins in this folder allow applications built on PhoneGap/Cordova
(just "PhoneGap" from here on out, for lack of a naming-neutral pronoun)
to use synchronize with Zumero for SQL Server (ZSS) data.

These instructions assume that you have installed the Android and/or
iOS PhoneGap SDK(s), available at http://phonegap.com/download/, and 
are comfortable adding source files and libraries to projects on 
those platforms.

PhoneGap 3 and 4 users will have an easier time of it. PhoneGap 2.x users, see below for manual installation details.


## PhoneGap 3 and 4 Installation

Assuming your SDK is unpacked to `/home/user/zumerosdk`, the plugin can be
found at `/home/user/zumerosdk/phonegap/plugin`. In that case, go to the root of your PhoneGap 3.x project and run:

    phonegap plugin add /home/user/zumerosdk/phonegap/plugin

then

    phonegap build ios

and/or

    phonegap build android

If you run:
  
    phonegap plugin list

you'll see `com.zumero.cordova` installed, alongside `com.zumero.plugins.sqlite`. That's the Zumero fork of the SQLite wrapper library, described in "References", below.

### Additional steps for iOS PhoneGap 3/4 projects
Add `ZumeroSync.framework` (in the SDK's `ios` folder) to your project's 
`Frameworks` list.

If you're targeting iOS 5, you'll need to bring your own 
SQLite implementation, as well.  See `ios/README.txt` for details.


## PhoneGap 2.x Installation - Android

Copy `ZumeroPlugin.java` (from `src/android`) into your project.  Edit the plugin file to comment out these imports:

    import org.apache.cordova.CallbackContext;
    import org.apache.cordova.CordovaPlugin;
    import org.apache.cordova.CordovaWebView;
    import org.apache.cordova.CordovaInterface;

and uncomment these:

    import org.apache.cordova.api.CallbackContext;
    import org.apache.cordova.api.CordovaPlugin;
    import org.apache.cordova.api.CordovaWebView;
    import org.apache.cordova.api.CordovaInterface;

Copy `zumero.js` (from `www`) to your `www` folder (or a
subfolder) and `require` it as needed. 

Copy the zumero.jar and the zumero *.so files, as described in the 
Android README.txt file.

Add this line to the `plugins` section in your `res/xml/config.xml` 
file:

    <plugin value="com.zumero.cordova.ZumeroPlugin" name="Zumero"/>

## PhoneGap 2.x Installation - iOS

Copy `ZumeroPlugin.m` and `ZumeroPlugin.h` (from `src/ios`) to your
Xcode project's Plugins folder. Add the plugin to your `config.xml`
like so:

    <plugin value="ZumeroPlugin" name="Zumero" />

Copy `zumero.js` (from `www`) to your `www` folder (or a
subfolder) and `require` it as needed. 

Add `ZumeroSync.framework` (in the SDK's `ios` folder) to your project's 
`Frameworks` list.

You'll also need to include the standard `zlib` and `CFNetwork` 
libraries; if you're targeting iOS 5, you'll need to bring your own 
SQLite implementation, as well.  See `ios/README.txt` for details.


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
`zumero_for_sql_server_client_api.pdf` in the SDK (or read online at
http://zumero.com/docs/zumero_for_sql_server_client_api.html)

Zumero needs the full path to the dbfile being synchronized; you'll either
need to know where your SQLite library stores its files (and if it changes
their filenames) and add that information to your Zumero calls, or use a
SQLite library that exposes the full name of an opened dbfile.

A Zumero-modified fork of the lite4cordova SQLite plugin can be found at
https://github.com/zumero/Cordova-SQLitePlugin - it extends the `openDatabase`
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
