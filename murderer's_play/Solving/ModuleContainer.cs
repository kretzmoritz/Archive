using UnityEngine;
using System.Collections;

public class ModuleContainer : MonoBehaviour
{
	private bool bShowModuleContainer = false;
	private DiaLog system_log;
	private Inventory system_inventory;
	private Solution system_solution;
	
	// Use this for initialization
	private void Start() 
	{
		this.transform.gameObject.AddComponent(typeof(DiaLog));
		system_log = this.transform.gameObject.GetComponent<DiaLog>();
		
		this.transform.gameObject.AddComponent(typeof(Inventory));
		system_inventory = this.transform.gameObject.GetComponent<Inventory>();
		
		this.transform.gameObject.AddComponent(typeof(Solution));
		system_solution = this.transform.gameObject.GetComponent<Solution>();
	}
	
	// Update is called once per frame
	private void Update()
	{
		
	}
	
	private void OnGUI()
	{
		if(bShowModuleContainer)
		{
			float fAspectRatioWidth = Screen.width / 1024.0f;
			float fAspectRatioHeight = Screen.height / 768.0f;
			
			GUIStyle style = GUI.skin.GetStyle("box");
			
			//Menu needs to be centered; leave some space on the edges
			GUILayout.BeginArea(new Rect(25 * fAspectRatioWidth, 25 * fAspectRatioHeight, 974 * fAspectRatioWidth, 718 * fAspectRatioHeight), style);
				if(GUI.Button(new Rect((974 * fAspectRatioWidth) - 25, 0, 25, 25), "X"))
				{
					this.transform.GetComponent<CreateInterface>().destroyModuleContainer(); //Quit module container
				}
			
				//Modules enabled
				system_log.show();
				system_inventory.show();
				system_solution.show();
			GUILayout.EndArea();
		}
	}
	
	public void show()
	{
		bShowModuleContainer = true;
	}
	
	public void hide()
	{
		bShowModuleContainer = false;
	}
}