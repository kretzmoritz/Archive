using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class item
{
	public Texture2D texLook;
	public string sInformation;
	public int iSolutionTag;
	public float fDrawLabel;
}

public class Inventory : MonoBehaviour
{
	private static Inventory singleton;
	public List<item> inventory_list = new List<item>();
	
	// Use this for initialization
	private void Start() 
	{
		if(singleton == null)
		{
			singleton = this;
		}
	}
	
	// Update is called once per frame
	private void Update()
	{
		for(int i = 0; i < inventory_list.Count; i++)
		{
			if(inventory_list[i].fDrawLabel > 0.0f)
			{
				inventory_list[i].fDrawLabel -= Time.deltaTime;
			}
		}
	}
	
	public void show()
	{
		float fAspectRatioWidth = Screen.width / 1024.0f;
		float fAspectRatioHeight = Screen.height / 768.0f;
		
		GUIStyle style = new GUIStyle();
		GUIContent content = new GUIContent();
		Vector2 vSize;
		
		GUI.Box(new Rect(306 * fAspectRatioWidth, 25 * fAspectRatioHeight, 643 * fAspectRatioWidth, 449.5f * fAspectRatioHeight), "Inventory");
		
		int iColumn = 0;
		int iRow = 0;
		for(int i = 0; i < inventory_list.Count; i++)
		{
			if(GUI.Button(new Rect((331 + iColumn * 85) * fAspectRatioWidth, (75 + iRow * 100) * fAspectRatioHeight, 75 * fAspectRatioWidth, 75 * fAspectRatioHeight), inventory_list[i].texLook))
			{
				if(Solution.get().bSolutionEnabled)
				{
					Solution.get().insertItem(inventory_list[i].sInformation, inventory_list[i].texLook, inventory_list[i].iSolutionTag);
				}
				else
				{
					for(int j = 0; j < inventory_list.Count; j++)
					{
						inventory_list[j].fDrawLabel = 0.0f; //On click disable all other labels shown
					}
					
					inventory_list[i].fDrawLabel = 10.0f; //Show for 10 seconds
				}
			}
			
			iColumn++;
			if(iColumn > 6) //0-6; 7 items allowed in one row
			{
				iColumn = 0;
				iRow++;
			}
			
			if(inventory_list[i].fDrawLabel > 0.0f)
			{
				style.fontSize = (int)(24 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
				style.fontStyle = FontStyle.Bold;
				style.normal.textColor = new Color(1, 1, 1, 1); //White text
				
				string sText = inventory_list[i].sInformation;
				content.text = sText;
				
				vSize = style.CalcSize(content);
				
				GUI.Label(new Rect(512 * fAspectRatioWidth, 384 * fAspectRatioHeight, vSize.x, vSize.y), content, style);
		
			}
		}
	}
	
	public static Inventory get()
	{
		if(singleton != null)
		{
			return singleton;
		}
		
		return null;
	}
}