/*
** Copyright 2013-2015 Zumero, LLC
** All rights reserved. 
 */

cordova.define("cordova/plugin/zumero", function(require, exports, module) {
	var exec = require('cordova/exec');
		
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
	
	
	//The last bit of cordova magic.
	var myplugin = new Zumero();
	module.exports = myplugin;
});
