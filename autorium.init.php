<?php
/*
Purpose: This file will call the initialisation file for the Autorium in case where the DB settings file is not
found, or the file is corrupted.
*/
file_put_contents('autorium.init.log',"Starting Init routine for Autorium.\n",FILE_APPEND);
// Rename the DB file if it exists
if(file_exists('autorium.settings.db')){
    file_put_contents('autorium.init.log',"Settings DB file already exists. Renaming it to avoid conflict.\n",FILE_APPEND);
    rename('autorium.settings.db',date('YmdHis') . ".autorium.settings.db");
}

// Create the DB
file_put_contents('autorium.init.log',"Creating the new settings DB file.\n",FILE_APPEND);
$init_db  = new SQLite3('autorium.settings.db');
$init_query = "CREATE TABLE autorium_settings (
    settingsId integer PRIMARY KEY,
    autoriumId text NOT NULL,
    aquariumShape integer NOT NULL,
    aquariumWidth integer,
    aquariumLength integer,
    aquariumHeight integer NOT NULL,
    aquariumDiameter integer,
    maxWaterLevel integer NOT NULL,
    minWaterLevel integer NOT NULL,
    autoriumCurrentState integer NOT NULL,
    autoriumOperationMode integer NOT NULL,
    waterLevel integer NOT NULL,
    minTemperature integer NOT NULL,
    maxTemperature integer NOT NULL,
   );";
$init_result = $init_db->query($init_query);

// Get the RPI serial number
file_put_contents('autorium.init.log',"Fetching RPI serial number.\n",FILE_APPEND);
$autoriumId = `cat /proc/cpuinfo | grep Serial | cut -d ' ' -f 2`;

// Store it in the settings DB
file_put_contents('autorium.init.log',"Storing RPI serial number in DB.\n",FILE_APPEND);
$init_result = $init_db->query("Insert into autorium_settings(settingsId, autoriumId ) values(1,'$autoriumId')");

// Register the autorium instance on the server
file_put_contents('autorium.init.log',"Registering the RPI on the server.\n",FILE_APPEND);
$autoriumRegisterData = file_get_contents("https://autorium.in/register.php?action=register&autorium_id=$autoriumId");
$autoriumJsonObj = json_decode($autoriumRegisterData);
file_put_contents('autorium.init.log',"Autorium Registeration result -> " . $autoriumJsonObj->{'autorium->result'} . " .\n",FILE_APPEND);

$init_db->close();

// Set Autorium init status to 2
if($autoriumJsonObj->{'autorium->result'}==0){
    $autoriumInit = 2;
    file_put_contents('autorium.init.log',"Autorium Registeration successful.\n",FILE_APPEND);
}else{
    file_put_contents('autorium.init.log',"Autorium Registeration not successful: " . $autoriumJsonObj->{'autorium->result'} . ".\n",FILE_APPEND);
}
    

// Note: Values for the DB needs to filled through app or web. The app needs to exit now and wiat for complete configuration.
echo "Basic initialisation of Autorium complete. Please configure the settings through App or web console to activate Autorium.";

?>