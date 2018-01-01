<?php
// Read local database for values that are stored.
 
if(file_exists('autorium.settings.db')){
    $settings_db  = new SQLite3('autorium.settings.db');
    $settings_query = "Select * from autorium_settings";
    $settings_result = $settings_db->query($settings_query);
    $settings_row        = $settings_result->fetchArray(); 
    if(count($settings_row)>0){
        $autoriumId = $settings_row['autoriumId']; 
        $aquariumShape = $settings_row['aquariumShape']; 
        $aquariumWidth = $settings_row['aquariumWidth']; 
        $aquariumLength = $settings_row['aquariumLength']; 
        $aquariumHeight = $settings_row['aquariumHeight']; 
        $aquariumDiameter = $settings_row['aquariumDiameter']; 
        $maxWaterLevel = $settings_row['maxWaterLevel']; 
        $minWaterLevel = $settings_row['minWaterLevel'];
        $autoriumOperationMode = $settings_row['autoriumOperationMode'];
        $waterLevel  = $settings_row['waterLevel'];
        $minTemperature = $settings_row['minTemperature']; 
        $maxTemperature = $settings_row['maxTemperature'];
    }
    else{
        // We cannot read data from the settings database, set the initialisation flag to 1
        $autoriumInit = 1;  
    }
    $settings_db->close();
    
} else{
    // We do not have the settings file. Need to run the setup script.
    file_put_contents('autorium.run.log',"Settings DB not found. Setting init flag\n",FILE_APPEND);
    $autoriumInit = 1;
}

?>