export function Name() { return "MOTE-V3"; }
export function Version() { return "1.1.9"; }
export function VendorId() { return 0x1981; }
export function ProductId() { return 0x1988; }
export function Publisher() { return "KPAX"; }
export function Documentation(){ return "qmk/srgbmods-qmk-firmware"; }
export function DeviceType() { return "keyboard"; }
export function ConflictingProcesses() { return ["VIA.exe"]; }
export function Size() { return [12, 6]; }
export function DefaultPosition(){return [10, 100]; }
export function DefaultScale(){return 8.0;}
/* global
shutdownMode:readonly
shutdownColor:readonly
LightingMode:readonly
forcedColor:readonly
*/
export function ControllableParameters() {
	return [
		{"property":"shutdownMode", "group":"lighting", "label":"Shutdown Mode", "type":"combobox", "values":["SignalRGB", "Hardware"], "default":"SignalRGB"},
		{"property":"shutdownColor", "group":"lighting", "label":"Shutdown Color", "min":"0", "max":"360", "type":"color", "default":"#000000"},
		{"property":"LightingMode", "group":"lighting", "label":"Lighting Mode", "type":"combobox", "values":["Canvas", "Forced"], "default":"Canvas"},
		{"property":"forcedColor", "group":"lighting", "label":"Forced Color", "min":"0", "max":"360", "type":"color", "default":"#009bde"},
	];
}

//Plugin Version: Built for Protocol V1.0.5

const vKeys = [
    0, 1, 2, 3, 4, 5,//L_row0
    6, 7, 8, 9, 10, 11,//L_row1
    12,13, 14, 15,16,17,//L_row2
    18, 19, 20, 21, 22, 23,//L_row3
            24, 25, 26, 27,//L_row4
    28, 29,30, 31, 32,	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,	 46,47, 48, 49,//L_underglow

	// 50, 51, 52, 53, 54, 55,//R_row0
    // 56, 57, 58, 59, 60, 61,//R_row1
	55, 54, 53, 52, 51, 50,//R_row0
    61, 60, 59, 58, 57, 56,//R_row1
    67, 66, 65, 64, 63, 62,//R_row2
    73, 72, 71, 70, 69, 68,//R_row3
    77, 76, 75,	74,//R_row4
	78, 79, 80, 81, 82, 83, 84,  85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,//R_underglow


];

const vKeyNames = [
    "Esc","1",  "2", "3", "4", "5",
	"Tab", "Q", "W", "E", "R", "T",
	"Shift", "A", "S", "D", "F", "G",
	"Left ctrl", "Z", "X", "C", "V", "B",
                "alt", "Space", "gui", "encoder",

	"L_U1","L_U2","L_U3","L_U4","L_U5","L_U6","L_U7","L_U8","L_U9","L_U10","L_U11","L_U12","L_U13","L_U14","L_U15","L_U16","L_U17","L_U18","L_U19","L_U20","L_U21","L_U22",

	"6", "7", "8", "9", "0",  "Backspace",
	"Y", "U", "I", "O", "P", "End",
    "H", "J", "K", "L", ";", "'",
	"N", "M", ",", ".", "/",  "Right ctl",
    "encoder", "mo6", "enter", "del",

    "R_U1", "R_U2","R_U3","R_U4","R_U5","R_U6","R_U7","R_U8","R_U9","R_U10","R_U11","R_U12","R_U13","R_U14","R_U15","R_U16","R_U17","R_U18","R_U19","R_U20","R_U21","R_U22",
];

const vKeyPositions = [
    [0, 0], [1, 0], [2, 0], [3, 0], [4, 0], [5, 0],
	[0, 1], [1, 1], [2, 1], [3, 1], [4, 1], [5, 1],
	[0, 2], [1, 2], [2, 2], [3, 2], [4, 2], [5, 2],
	[0, 3], [1, 3], [2, 3], [3, 3], [4, 3], [5, 3],
	     	        [2, 4], [3, 4], [4, 4], [5, 4],
	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],	[0, 5],

	[6, 0], [7, 0], [8, 0], [9, 0], [10, 0], [11, 0],
	[6, 1], [7, 1], [8, 1], [9, 1], [10, 1], [11, 1],
	[6, 2], [7, 2], [8, 2], [9, 2], [10, 2], [11, 2],
	[6, 3], [7, 3], [8, 3], [9, 3], [10, 3], [11, 3],
	[6, 4], [7, 4], [8, 4], [9, 4],
    [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5], [6, 5],


];

// const Keyboard = {
// 	Left : {
// 		devicename: "Left Side",
// 		ledids: [
// 			0, 1, 2, 3,	4, 5,
// 			12,13,14,15,16,17,
//             24,25,26,27,28,29,
//             36,37,38,39,40,41,
//                         48,49,50,51,
//             56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,


// 		],
// 		lednames: [
// 			"Esc",   "1",  "2", "3", "4", "5",
// 			"Tab",   "Q",  "W", "E", "R", "T",
// 			"Shift", "A",  "S", "D", "F", "G",
//             "Ctrl",  "Z",  "X", "C", "V", "B",
//                                    "Left Alt", "Space","Apps","AudioMute",
//             // "ug1", "ug2","ug3","ug4","ug5","ug6","ug7","ug8","ug9","ug10","ug11","ug12","ug13","ug14","ug15","ug16","ug17","ug18","ug19","ug20","ug21","ug22",

// 		],
// 		ledpos:	[
// 			[0, 0], [1, 0], [2, 0], [3, 0],[4, 0],[5, 0],
// 			[0, 1], [1, 1], [2, 1], [3, 1],[4, 1],[5, 1],
// 			[0, 2], [1, 2], [2, 2], [3, 2],[4, 2],[5, 2],
//             [0, 3], [1, 3], [2, 3], [3, 3],[4, 3],[5, 3],
// 			                               [4, 4],[5, 4],[6, 4],[6, 3],
//             // [5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],

// 		],
// 		width: 7,
// 		height: 5,
// 		image: Image()
// 	},
// 	Right : {
// 		devicename: "Right Side",
// 		ledids: [
//             6, 7, 8, 9, 10, 11,
//             18,19,20,21,22,23,
//             30,31,32,33,34,35,
//             42,43,44,45,46,47,
// 	  52,53,54,55,
//          78,79, 80, 81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,

// 		],
// 		lednames: [
// 			"Backspace",   "0",  "9", "8", "7", "6",
// 			"Tab",   "P",  "O", "I", "U", "Y",
// 			"Shift", ";",  "L", "K", "J", "H",
//             "Ctrl",  "/",  ".", ",", "M", "N",
//             "Left Alt", "Space","Apps","AudioMute",
//             // "ug1", "ug2","ug3","ug4","ug5","ug6","ug7","ug8","ug9","ug10","ug11","ug12","ug13","ug14","ug15","ug16","ug17","ug18","ug19","ug20","ug21","ug22",
// 		],
// 		ledpos:	[
// 			[0, 0], [1, 0], [2, 0], [3, 0],[4, 0],[5, 0],
// 			[0, 1], [1, 1], [2, 1], [3, 1],[4, 1],[5, 1],
// 			[0, 2], [1, 2], [2, 2], [3, 2],[4, 2],[5, 2],
//             [0, 3], [1, 3], [2, 3], [3, 3],[4, 3],[5, 3],
// 			                               [4, 4],[5, 4],[6, 4],[6, 3],
//             [5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],[5, 2],[5, 3],[5, 4],[5, 0],[5, 1],
// 			 [5, 0], [4, 0],[3, 0], [2, 0],[1, 0],[0, 0],
// 			 [5, 1], [4, 1],[3, 1], [2, 1],[1, 1],[0, 1],
// 			 [5, 2], [4, 2],[3, 2], [2, 2],[1, 2],[0, 2],
//              [5, 3], [4, 3],[3, 3], [2, 3],[1, 3],[0, 3],
//              [6, 3],[6, 4],[5, 4],[4, 4],
//             [4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],[4, 4],
// 		],
// 		width: 7,
// 		height: 5,
// 		image: Image()
// 	}
// };

let LEDCount = 0;
let IsViaKeyboard = false;
const MainlineQMKFirmware = 1;
const VIAFirmware = 2;
const PluginProtocolVersion = "1.0.5";

export function LedNames() {
	return vKeyNames;
}

export function LedPositions() {
	return vKeyPositions;
}

export function vKeysArrayCount() {
	device.log('vKeys ' + vKeys.length);
	device.log('vKeyNames ' + vKeyNames.length);
	device.log('vKeyPositions ' + vKeyPositions.length);
}

export function Initialize() {
	requestFirmwareType();
	requestQMKVersion();
	requestSignalRGBProtocolVersion();
	requestUniqueIdentifier();
	requestTotalLeds();
	effectEnable();

}

export function Render() {
	sendColors();
}

export function Shutdown(SystemSuspending) {

	if(SystemSuspending) {
		sendColors("#000000"); // Go Dark on System Sleep/Shutdown
	} else {
		if (shutdownMode === "SignalRGB") {
			sendColors(shutdownColor);
		} else {
			effectDisable();
		}
	}

	vKeysArrayCount(); // For debugging array counts

}

function commandHandler() {
	const readCounts = [];

	do {
		const returnpacket = device.read([0x00], 32, 10);
		processCommands(returnpacket);

		readCounts.push(device.getLastReadSize());

		// Extra Read to throw away empty packets from Via
		// Via always sends a second packet with the same Command Id.
		if(IsViaKeyboard) {
			device.read([0x00], 32, 10);
		}
	}
	while(device.getLastReadSize() > 0);

}

function processCommands(data) {
	switch(data[1]) {
	case 0x21:
		returnQMKVersion(data);
		break;
	case 0x22:
		returnSignalRGBProtocolVersion(data);
		break;
	case 0x23:
		returnUniqueIdentifier(data);
		break;
	case 0x24:
		sendColors();
		break;
	case 0x27:
		returnTotalLeds(data);
		break;
	case 0x28:
		returnFirmwareType(data);
		break;
	}
}

function requestQMKVersion() //Check the version of QMK Firmware that the keyboard is running
{
	device.write([0x00, 0x21], 32);
	device.pause(30);
	commandHandler();
}

function returnQMKVersion(data) {
	const QMKVersionByte1 = data[2];
	const QMKVersionByte2 = data[3];
	const QMKVersionByte3 = data[4];
	device.log("QMK Version: " + QMKVersionByte1 + "." + QMKVersionByte2 + "." + QMKVersionByte3);
	device.log("QMK SRGB Plugin Version: "+ Version());
	device.pause(30);
}

function requestSignalRGBProtocolVersion() //Grab the version of the SignalRGB Protocol the keyboard is running
{
	device.write([0x00, 0x22], 32);
	device.pause(30);
	commandHandler();
}

function returnSignalRGBProtocolVersion(data) {
	const ProtocolVersionByte1 = data[2];
	const ProtocolVersionByte2 = data[3];
	const ProtocolVersionByte3 = data[4];

	const SignalRGBProtocolVersion = ProtocolVersionByte1 + "." + ProtocolVersionByte2 + "." + ProtocolVersionByte3;
	device.log(`SignalRGB Protocol Version: ${SignalRGBProtocolVersion}`);


	if(PluginProtocolVersion !== SignalRGBProtocolVersion) {
		device.notify("Unsupported Protocol Version", `This plugin is intended for SignalRGB Protocol version ${PluginProtocolVersion}. This device is version: ${SignalRGBProtocolVersion}`, 2, "Documentation");
	}

	device.pause(30);
}

function requestUniqueIdentifier() //Grab the unique identifier for this keyboard model
{
	if(device.write([0x00, 0x23], 32) === -1) {
		device.notify("Unsupported Firmware", "This device is not running SignalRGB-compatible firmware. Click the Documentation button to learn more.", 3, "Documentation");
	}

	device.pause(30);
	commandHandler();
}


function returnUniqueIdentifier(data) {
	const UniqueIdentifierByte1 = data[2];
	const UniqueIdentifierByte2 = data[3];
	const UniqueIdentifierByte3 = data[4];

	if(!(UniqueIdentifierByte1 === 0 && UniqueIdentifierByte2 === 0 && UniqueIdentifierByte3 === 0)) {
		device.log("Unique Device Identifier: " + UniqueIdentifierByte1 + UniqueIdentifierByte2 + UniqueIdentifierByte3);
	}

	device.pause(30);
}

function requestTotalLeds() //Calculate total number of LEDs
{
	device.write([0x00, 0x27], 32);
	device.pause(30);
	commandHandler();
}

function returnTotalLeds(data) {
	LEDCount = data[2];
	device.log("Device Total LED Count: " + LEDCount);
	device.pause(30);
}

function requestFirmwareType() {
	device.write([0x00, 0x28], 32);
	device.pause(30);
	commandHandler();
}

function returnFirmwareType(data) {
	const FirmwareTypeByte = data[2];

	if(!(FirmwareTypeByte === MainlineQMKFirmware || FirmwareTypeByte === VIAFirmware)) {
		device.notify("Unsupported Firmware", "Click the Documentation button to learn more.", 3, "Documentation");
	}

	if(FirmwareTypeByte === MainlineQMKFirmware) {
		IsViaKeyboard = false;
		device.log("Firmware Type: Mainline");
	}

	if(FirmwareTypeByte === VIAFirmware) {
		IsViaKeyboard = true;
		device.log("Firmware Type: VIA");
	}

	device.pause(30);
}

function effectEnable() //Enable the SignalRGB Effect Mode
{
	device.write([0x00, 0x25], 32);
	device.pause(30);
}

function effectDisable() //Revert to Hardware Mode
{
	device.write([0x00, 0x26], 32);
	device.pause(30);
}

function createSolidColorArray(color) {
	const rgbdata = new Array(vKeys.length * 3).fill(0);

	for(let iIdx = 0; iIdx < vKeys.length; iIdx++) {
		const iLedIdx = vKeys[iIdx] * 3;
		rgbdata[iLedIdx] = color[0];
		rgbdata[iLedIdx+1] = color[1];
		rgbdata[iLedIdx+2] = color[2];
	}

	return rgbdata;
}

function grabColors(overrideColor) {
	if(overrideColor) {
		return createSolidColorArray(hexToRgb(overrideColor));
	} else if (LightingMode === "Forced") {
		return createSolidColorArray(hexToRgb(forcedColor));
	}

	const rgbdata = new Array(vKeys.length * 3).fill(0);

	for(let iIdx = 0; iIdx < vKeys.length; iIdx++) {
		const iPxX = vKeyPositions[iIdx][0];
		const iPxY = vKeyPositions[iIdx][1];
		const color = device.color(iPxX, iPxY);

		const iLedIdx = vKeys[iIdx] * 3;
		rgbdata[iLedIdx] = color[0];
		rgbdata[iLedIdx+1] = color[1];
		rgbdata[iLedIdx+2] = color[2];
	}

	return rgbdata;
}

function sendColors(overrideColor) {
	const rgbdata = grabColors(overrideColor);

	const LedsPerPacket = 9;
	let BytesSent = 0;
	let BytesLeft = rgbdata.length;

	while(BytesLeft > 0) {
		const BytesToSend = Math.min(LedsPerPacket * 3, BytesLeft);
		StreamLightingData(Math.floor(BytesSent / 3), rgbdata.splice(0, BytesToSend));

		BytesLeft -= BytesToSend;
		BytesSent += BytesToSend;
	}
}

function StreamLightingData(StartLedIdx, RGBData) {
	const packet = [0x00, 0x24, StartLedIdx, Math.floor(RGBData.length / 3)].concat(RGBData);
	device.write(packet, 33);
}

function hexToRgb(hex) {
	const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	const colors = [];
	colors[0] = parseInt(result[1], 16);
	colors[1] = parseInt(result[2], 16);
	colors[2] = parseInt(result[3], 16);

	return colors;
}

export function Validate(endpoint) {
	return endpoint.interface === 1;
}

export function Image() {
	return "";
}
