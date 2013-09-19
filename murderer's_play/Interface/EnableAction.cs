using UnityEngine;
using System.Collections;
using System.Collections.Generic;

[System.Serializable]
public class list_string
{
	public List<string> strings = new List<string>();
	
	public string this[int i]
	{
	    get { return strings[i]; }
	    set { strings[i] = value; }
	}
	
	public int Count
	{
	    get { return strings.Count; }
	}
}

[System.Serializable]
public class list_ints
{
	public List<int> ints = new List<int>();
	
	public int this[int i]
	{
	    get { return ints[i]; }
	    set { ints[i] = value; }
	}
	
	public int Count
	{
	    get { return ints.Count; }
	}
}

public class EnableAction : MonoBehaviour
{
	private bool bShowLabelActor = false;
	private bool bShowLabelItem = false;
	
	private RaycastHit rayHitInfo;
	private CreateInterface interface_handler;
	
	public bool bHorizontal = true;
	public int iButtonSpacing = 50;
	
	public string sDialogButton = "e";
	public string sItemButton = "z";
	public string sModuleContainerButton = "escape";
	
	public List<list_string> sCorrectSolutionLabels = new List<list_string>();
	public List<list_ints> iCorrectSolutionIDs = new List<list_ints>();
	
	// Use this for initialization
	private void Start()
	{
		this.transform.gameObject.AddComponent(typeof(CreateInterface));
		interface_handler = this.transform.gameObject.GetComponent<CreateInterface>();
	}
	
	// Update is called once per frame
	private void Update()
	{
		if(interface_handler.getState() == state.Clear)
		{
			//Show ray
			Debug.DrawLine(this.transform.FindChild("Main Camera").transform.position, this.transform.FindChild("Main Camera").transform.position + this.transform.FindChild("Main Camera").transform.forward.normalized * 2.0f);
			
			if(Physics.Raycast(this.transform.FindChild("Main Camera").transform.position, this.transform.FindChild("Main Camera").transform.forward, out rayHitInfo, 2.0f))
			{
				if(rayHitInfo.transform.tag == "Actor" && rayHitInfo.transform.GetComponent<ConversationContainer>().bConversationEnabled)
				{
					bShowLabelActor = true;
					
					if(Input.GetKeyDown(sDialogButton))
					{
						ConversationContainer temp = rayHitInfo.transform.GetComponent<ConversationContainer>();
						interface_handler.setDialog(temp.sName, temp.conversation); //Fill our interface class with the correct dialog information
						interface_handler.setSettings(bHorizontal, iButtonSpacing);
						interface_handler.createDialog();
						
						rayHitInfo.transform.GetComponent<ConversationContainer>().bConversationEnabled = false; //We already had a conversation with this actor
					}
				}
				else if(rayHitInfo.transform.tag == "Item")
				{
					bShowLabelItem = true;
					
					if(Input.GetKeyDown(sItemButton))
					{
						rayHitInfo.transform.position = new Vector3(15000.0f, 15000.0f, 15000.0f); //Move this object out of view
						rayHitInfo.transform.localScale = new Vector3(rayHitInfo.transform.localScale.x * 20.0f, rayHitInfo.transform.localScale.y * 20.0f, rayHitInfo.transform.localScale.z * 20.0f);
						
						item _temp = new item();
						_temp.sInformation = rayHitInfo.transform.GetComponent<ItemContainer>().sInformation;
						_temp.iSolutionTag = rayHitInfo.transform.GetComponent<ItemContainer>().iSolutionTag;
						_temp.fDrawLabel = 0.0f;
						
						RenderTexture inventory_object = new RenderTexture(256, 256, 24);
				        inventory_object.isPowerOfTwo = true;
						inventory_object.Create();
						
						Color cAlpha = new Color(1.0f, 1.0f, 1.0f, 1.0f);
						GameObject camObject = new GameObject();
						camObject.AddComponent<Camera>();
						camObject.transform.parent = rayHitInfo.transform;
						camObject.GetComponent<Camera>().backgroundColor = cAlpha;
						camObject.GetComponent<Camera>().targetTexture = inventory_object;
						camObject.transform.position = new Vector3(15000.0f, 15000.0f + 1.25f, 15000.0f - 7.5f);
						
						RenderTexture.active = inventory_object;
						camObject.GetComponent<Camera>().Render();
						
						_temp.texLook = new Texture2D(256, 256, TextureFormat.ARGB32, true);
						_temp.texLook.ReadPixels(new Rect(0, 0, inventory_object.width, inventory_object.height), 0, 0);
						_temp.texLook.Apply();
						
						Color[] aPixel = _temp.texLook.GetPixels();
						for(int i = 0; i < aPixel.Length; i++)
						{
							if(aPixel[i].r == 1.0f && aPixel[i].g == 1.0f && aPixel[i].b == 1.0f && aPixel[i].a == 1.0f)
							{
								aPixel[i].a = 0.0f;
							}
						}
						_temp.texLook.SetPixels(aPixel);
						_temp.texLook.Apply();
						
						RenderTexture.active = null;
						Destroy(camObject);
						Destroy(rayHitInfo.transform.gameObject);
						
						Inventory.get().inventory_list.Add(_temp);
					}
				}
			}
			else
			{
				bShowLabelActor = false;
				bShowLabelItem = false;
			}
			
			if(Input.GetKeyDown(sModuleContainerButton) && interface_handler.getState() == state.Clear)
			{
				interface_handler.createModuleContainer();
			}
		}
		else
		{
			if(Input.GetKeyDown(sModuleContainerButton) && interface_handler.getState() == state.Module)
			{
				interface_handler.destroyModuleContainer();
			}
			
			bShowLabelActor = false; //An interface element is active -> don't show the label
			bShowLabelItem = false;
		}
	}
	
	private void OnGUI()
	{
		float fAspectRatioWidth = Screen.width / 1024.0f;
		float fAspectRatioHeight = Screen.height / 768.0f;
		
		GUIStyle style = new GUIStyle();
		GUIContent content = new GUIContent();
		Vector2 vSize;
		
		if(bShowLabelActor)
		{
			style.fontSize = (int)(48 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
			style.fontStyle = FontStyle.Bold;
			style.normal.textColor = new Color(1, 1, 1, 1); //White text
			
			string sText = "Press "+sDialogButton.ToUpper()+" to talk!";
			content.text = sText;
			
			vSize = style.CalcSize(content);
			
			GUI.Label(new Rect(512 * fAspectRatioWidth, 384 * fAspectRatioHeight, vSize.x, vSize.y), content, style);
		}
		else if(bShowLabelItem)
		{
			style.fontSize = (int)(48 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
			style.fontStyle = FontStyle.Bold;
			style.normal.textColor = new Color(1, 1, 1, 1); //White text
			
			string sText = "Press "+sItemButton.ToUpper()+" to pick up!";
			content.text = sText;
			
			vSize = style.CalcSize(content);
			
			GUI.Label(new Rect(512 * fAspectRatioWidth, 384 * fAspectRatioHeight, vSize.x, vSize.y), content, style);
		}
	}
}