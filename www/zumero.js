/*
** Copyright 2013-2017 Zumero, LLC
** All rights reserved. 
 */
var zumero_global_progress_callbacks = {}
function zumero_global_progress_callback_function(callback_key, cancellation_token, phase, bytesSoFar, bytesTotal) {
    if (callback_key in zumero_global_progress_callbacks && zumero_global_progress_callbacks[callback_key] != undefined) {
		zumero_global_progress_callbacks[callback_key](cancellation_token, phase, bytesSoFar, bytesTotal);
	}
	if (phase == 5)	//Phase 5 is APPLYING, which is the last phase.
		delete zumero_global_progress_callbacks[callback_key]; 
}

cordova.define("cordova/plugin/zumero", function(require, exports, module) {
	var exec = require('cordova/exec');

	var win = function(message) {
		eval(message);
	};

	exec(win, null, 'Zumero', 'setupJSPassthrough', []);

	//The cordova module object.
	var Zumero = function() {
	};
		
	Zumero.prototype.sync = function(fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password, syncSuccessCallback, syncErrorCallback) {
		try {	
			exec(syncSuccessCallback, syncErrorCallback, "Zumero", "sync", [ fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}		
	
	Zumero.prototype.sync2 = function(fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password, progressCallback, syncSuccessCallback, syncErrorCallback) {
               var random = Math.floor(Math.random() * 100000);
		zumero_global_progress_callbacks[random] = progressCallback;
		try {	
			exec(syncSuccessCallback, syncErrorCallback, "Zumero", "sync2", [ fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password, random ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}		

	Zumero.prototype.sync3 = function(fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password, jsOptions, progressCallback, syncSuccessCallback, syncErrorCallback) {
               var random = Math.floor(Math.random() * 100000);
		zumero_global_progress_callbacks[random] = progressCallback;
		try {	
			exec(syncSuccessCallback, syncErrorCallback, "Zumero", "sync3", [ fullPath, encryptionKey, serverURL, dbFileName, scheme, user, password, random, jsOptions ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}		

	Zumero.prototype.quarantineSinceLastSync = function(fullPath, encryptionKey, successCallback, errorCallback) {
		try {	
			exec(function(arg) {
					successCallback(arg.quarantineID);
				}, errorCallback, "Zumero", "quarantineSinceLastSync", [ fullPath, encryptionKey ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}		

	Zumero.prototype.syncQuarantine = function(fullPath, encryptionKey, quarantineID, serverURL, dbFileName, scheme, user, password, successCallback, errorCallback) {
		try {	
			exec(successCallback, errorCallback, "Zumero", "syncQuarantine", [ fullPath, encryptionKey, quarantineID, serverURL, dbFileName, scheme, user, password ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}

	
	Zumero.prototype.deleteQuarantine = function(fullPath, encryptionKey, quarantineID, successCallback, errorCallback) {
		try {	
			exec(successCallback, errorCallback, "Zumero", "deleteQuarantine", [ fullPath, encryptionKey, quarantineID ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}


	Zumero.prototype.cancel = function(cancel_token, successCallback, errorCallback) {
		try {	
			exec(successCallback, errorCallback, "Zumero", "cancel", [ cancel_token ]);
		}
		catch (err) {
			console.log("-----------------------  Caught Exception   -----------------------------------");
			console.log(err);
		}
		finally {
		}
	}
	
	
	//The last bit of cordova magic.
	var myplugin = new Zumero();
	module.exports = myplugin;
});
