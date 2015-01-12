<?php 

namespace CVWO\HelloWorld {
	require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

	/**
	 * Displays the form for searching the user records
	 */
	function helloworld_upload_mass_add_form($form, &$form_state,$page_mode){
	
		$form['file'] = array(
    		'#type' => 'file',
    		'#title' => t('Text file'),
    		'#description' => t('Upload a file, allowed extensions: txt,csv'),
  		);
  		$form['submit'] = array(
    		'#type' => 'submit',
    		'#value' => t('Submit'),
  		);

  
		return $form;
	}
	function helloworld_upload_mass_add_form_validate($form, &$form_state) {
  		$file = file_save_upload('file', array(
    
    	// Validates file is text.
    	'file_validate_is_text' => array(),
    
    	// Validate extensions.
    	'file_validate_extensions' => array('txt csv'),
  		));
  		// If the file passed validation:
  		if ($file) {
    	// Move the file into the Drupal file system.
    		if ($file = file_move($file, 'public://')) {
      	// Save the file for use in the submit handler.
      			$form_state['storage']['file'] = $file;
    		}
    		else {
      		form_set_error('file', t("Failed to write the uploaded file to the site's file folder."));
    		}
  		}
  		else {
    		form_set_error('file', t('No file was uploaded.'));
  		}
	}
	/**
	 * Submit handler for helloworld_mass_add_form
	 * Redirects the page along with parameters, when submit button is pressed
	 */
	function helloworld_upload_mass_add_form_submit($form, &$form_state){
		$file = $form_state['storage']['file'];
		$realpath = drupal_realpath($file->uri);
 		$fh = fopen($realpath, 'r');
 		$contents = fread($fh,filesize($realpath));
		$mass_add = preg_split("/$\R?^/m",$contents,-1,PREG_SPLIT_NO_EMPTY);
		$error = FALSE;
		foreach ($mass_add as $line)
		{
			$new_values = explode(",",$line);
			
			
			$new_values[1]= isset($new_values[1]) ? $new_values[1] : null; 
			$value = array(
				'name' => trim($new_values[0]),
				'email' => trim($new_values[1]),
				'comment' => '',
			);
			if(isset($form_state['values']['comment']))
			{
				$value['comment'] = $form_state['values']['comment'];
			}
			if(!valid_email_address($value['email']))
			{	
				drupal_set_message(t('An error (Invalid Email) was encountered while adding %name', array('%name' => $value['name'])),'error');
			

				
				$error= TRUE;
			}
			else
			{
				if (add_user($value)) 
				{
					drupal_set_message(t('%name has been successfully added', array('%name' => $value['name'])));
				} 
				else 
				{
					drupal_set_message(t('An error was encountered while adding %name', array('%name' => $value['name'])),'error');
				
					
					$error = TRUE;
				}
			}
		}
		 if ($error) {
			$form_state['rebuild'] = TRUE;
		} else {
		
			$form_state['redirect'] = MODULE_NAME;
		}
	}

	
}

