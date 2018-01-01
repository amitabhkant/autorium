<?php
/*
Purpose: This file will call the initialisation file for the Autorium in case where the DB settings file is not
found, or the file is corrupted.
*/
file_put_contents('autorium.init.log',"\n\n*****************************\n",FILE_APPEND);
file_put_contents('autorium.init.log',"Autorium Init Log: " . date('Y-m-d H:i:s') . "\n",FILE_APPEND);
file_put_contents('autorium.init.log',"Starting Init routine for Autorium\n",FILE_APPEND);
// Rename the DB file if it exists
if(file_exists('autorium.settings.db')){
    file_put_contents('autorium.init.log',"Settings DB file already exists. Renaming it to avoid conflict\n",FILE_APPEND);
    rename('autorium.settings.db',date('YmdHis') . ".autorium.settings.db");
}

// Create the DB
file_put_contents('autorium.init.log',"Creating new settings DB file\n",FILE_APPEND);
$init_db  = new SQLite3('autorium.settings.db');
$init_query = "CREATE TABLE autorium_settings (
    settingsId integer PRIMARY KEY,
    autoriumId text,
    aquariumShape integer,
    aquariumWidth integer,
    aquariumLength integer,
    aquariumHeight integer,
    aquariumDiameter integer,
    maxWaterLevel integer,
    minWaterLevel integer,
    autoriumOperationMode integer,
    waterLevel integer,
    minTemperature integer,
    maxTemperature integer
   );";
$init_result = $init_db->query($init_query);

// Get the RPI serial number
file_put_contents('autorium.init.log',"Fetching Autorium serial number\n",FILE_APPEND);
$autoriumId = `cat /proc/cpuinfo | grep Serial | cut -d ' ' -f 2`;
$autoriumId =  trim($autoriumId);

// Store it in the settings DB
file_put_contents('autorium.init.log',"Storing Autorium serial number {$autoriumId} in DB\n",FILE_APPEND);
$init_result = $init_db->query("Insert into autorium_settings(settingsId, autoriumId, autoriumOperationMode, minTemperature, maxTemperature) values(1,'$autoriumId',0,20,32)");

// Register the autorium instance on the server
file_put_contents('autorium.init.log',"Registering Autorium on the server\n",FILE_APPEND);
//$autoriumRegisterData = file_get_contents("https://autorium.in/register.php?action=register&autorium_id=$autoriumId");
$autoriumRegisterData = file_get_contents("https://gorillastudios.in/autorium/?action=register&autorium_id=$autoriumId");
if($autoriumRegisterData===false){
    // Unable to contact server. Reverse back all the changes
    file_put_contents('autorium.init.log',"Unable to register Autorium on the server. Reverting back the changes and exiting init routine\n",FILE_APPEND);
    unlink('autorium.settings.db');
    die();
}
$autoriumJsonObj = json_decode($autoriumRegisterData);
file_put_contents('autorium.init.log',"Autorium Registration result -> " . $autoriumJsonObj->{'status_code'} . " \n",FILE_APPEND);

$init_db->close();

// Set Autorium init status to 2
if($autoriumJsonObj->{'status_code'}==0){
    $autoriumInit = 2;
    file_put_contents('autorium.init.log',"Autorium Registration successful\n",FILE_APPEND);
}else{
    file_put_contents('autorium.init.log',"Autorium Registration not successful: " . $autoriumJsonObj->{'status_code'} . "\n",FILE_APPEND);
}
file_put_contents('autorium.init.log',"Basic initialisation of Autorium complete on " . date('Y-m-d H:i:s') . ". Please configure further settings through App or web console to activate Autorium\n",FILE_APPEND);
// Note: Values for the DB needs to filled through app or web. The app needs to exit now and wait for complete configuration.
?>