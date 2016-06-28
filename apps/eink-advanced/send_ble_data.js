var noble = require('noble');

var text = process.argv[2];
var buf = Buffer.from(text);

var text_value = "e528a410ff4f3089d44f7cb505aba641";
var scale =      "e528a413ff4f3089d44f7cb505aba641";
var x =          "e528a411ff4f3089d44f7cb505aba641";
var y =          "e528a412ff4f3089d44f7cb505aba641";
var qrcode =     "e528a414ff4f3089d44f7cb505aba641";

noble.on('stateChange', function(state) {
	if(state === 'poweredOn'){
		noble.startScanning();
	}else{
		noble.stopScanning();
	}
});

noble.on('discover', function(peripheral){
	if(peripheral.address === "c0:98:e5:00:f8:02" && peripheral.connectable)
	{
		peripheral.connect(function(error){
			if(error)
			{
				console.log("Peripheral connection error: " + error);
			}
			else
			{
				peripheral.discoverAllServicesAndCharacteristics(function(error, services, characteristics){
					//characteristics[0] = X COORDINATE
					//characteristics[1] = Y COORDINATE
					//characteristics[2] = SCALE
					//characteristics[3] = TEXT
					//characteristics[4] = QR CODE
					for(var i = 0; i < characteristics.length; i++)
					{
						if(characteristics[i].uuid == text_value)
						{
							characteristics[i].write(buf, false, function(error){
								//console.log("Characteristics error: " + error);
							});
						}
					}
				});
			}

		});
	}
});
