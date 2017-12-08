<?php
/*
Purpose: This is the main entry file for Autorium, when using Raspberry as base board. This file will 
be calling all other files, whether python scripts for GPIO or native binaries for other functions.
Created by: Amitabh Kant
Date: 22nd Nov 2017
*/

// Include the constant/variable definition file
require_once('autorium.constants.php');

// Include the settings file. If the settings database file is not found, the init routine should be
// called immediately
require_once('autorium.settings.php');

if($autoriumInit==1){
    // It seems that either the settings DB is not present or the DB is currupted. Run the Init section
    require_once('autorium.init.php');
}



?>