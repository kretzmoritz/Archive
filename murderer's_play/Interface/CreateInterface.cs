using UnityEngine;
using System.Collections;

public enum state
{
	Clear = 0,
	Dialog = 1,
	Module = 2
}

public class CreateInterface : MonoBehaviour
{
	private state show_interface = state.Clear;
	
	// Use this for initialization
	private void Start()
	{
		this.transform.gameObject.AddComponent(typeof(CreateDialog));
		this.transform.gameObject.AddComponent(typeof(ModuleContainer));
	}
	
	// Update is called once per frame
	private void Update()
	{
		
	}
	
	public void setSettings(bool _bAlign, int _iSpacing)
	{
		this.transform.gameObject.GetComponent<CreateDialog>().setSettings(_bAlign, _iSpacing);
	}
	
	public void setDialog(string _name, dialog[] _input)
	{
		this.transform.gameObject.GetComponent<CreateDialog>().setDialog(_name, _input);
	}
	
	public void createDialog()
	{
		this.transform.gameObject.GetComponent<CreateDialog>().show();
		show_interface = state.Dialog;
		
		disableCamera();
	}
	
	public void destroyDialog()
	{
		this.transform.gameObject.GetComponent<CreateDialog>().hide();
		show_interface = state.Clear;
		
		enableCamera();
	}
	
	public void createModuleContainer()
	{
		this.transform.gameObject.GetComponent<ModuleContainer>().show();
		show_interface = state.Module;
		
		disableCamera();
	}
	
	public void destroyModuleContainer()
	{
		this.transform.gameObject.GetComponent<ModuleContainer>().hide();
		show_interface = state.Clear;
		
		enableCamera();
	}
	
	public state getState()
	{
		return show_interface;
	}
	
	private void enableCamera()
	{
		//Enable camera movement
		this.GetComponent<CharacterMotor>().enabled = true;
		this.GetComponent<MouseLook>().enabled = true;
		this.transform.FindChild("Main Camera").camera.GetComponent<MouseLook>().enabled = true;
	}

	private void disableCamera()
	{
		//Disable camera movement
		this.GetComponent<CharacterMotor>().enabled = false;
		this.GetComponent<MouseLook>().enabled = false;
		this.transform.FindChild("Main Camera").camera.GetComponent<MouseLook>().enabled = false;
	}
}