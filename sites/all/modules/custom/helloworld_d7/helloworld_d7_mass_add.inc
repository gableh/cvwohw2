<?php 

namespace CVWO\HelloWorld {
	require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

	/**
	 * Displays the form for searching the user records
	 */
	function helloworld_mass_add_form($form, &$form_state,$page_mode){

		if (isset($form_state['values'])) {
			$values = $form_state['values'];
		} else {
			$values = array(
				'mass_add' => '',
				'comment' => '',
			);
		}
		if ($page_mode == 'view') {
			$disabled = TRUE;
		} else {
			$disabled = FALSE;
		}
		
		$form['mass_add'] = array(
			'#type' => 'textarea',
			'#title' => t('Mass Add'),
			'#disabled' => $disabled,
			'#default_value' => $values['mass_add'],
			'#rows' => 3,
			'#required' => TRUE,
		);
		
		
		$form['comment'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
			'#disabled' => $disabled,
			
			'#rows' => 1,
			'#required' => FALSE,
		);
		$form['mass_add_submit'] = array(
			'#type' => 'submit',
			'#value' => t('Add all'),
		);
		return $form;
	}

	/**
	 * Submit handler for helloworld_mass_add_form
	 * Redirects the page along with parameters, when submit button is pressed
	 */
	function helloworld_mass_add_form_submit($form, &$form_state){
		$mass_add = preg_split("/$\R?^/m",$form_state['values']['mass_add'],-1,PREG_SPLIT_NO_EMPTY);
		
		$error = FALSE;
		foreach ($mass_add as $line)
		{
			$new_values = explode(",",$line);
			
			
			$new_values[1]= isset($new_values[1]) ? $new_values[1] : null; 
			$value = array(
				'name' => $new_values[0],
				'email' => $new_values[1],
				'comment' => '',
			);
			if(isset($form_state['values']['comment']))
			{
				$value['comment'] = $form_state['values']['comment'];
			}
			if(!valid_email_address($value['email']))
			{	
				drupal_set_message(t(valid_email_address($value['email'])));
				
				drupal_set_message(t('An error (Invalid Email) was encountered while adding %name', array('%name' => $value['name'])));
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
					drupal_set_message(t('An error was encountered while adding %name', array('%name' => $value['name'])));
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
