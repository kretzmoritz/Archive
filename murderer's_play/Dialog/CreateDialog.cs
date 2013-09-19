using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CreateDialog : MonoBehaviour
{
	public bool bShowDialog = false;
	
	private int iDialogIndex;
	private string sName;
	private dialog[] conversation;
	
	private float fCurrentOffset = 0.0f;
	private float fTotalAnswerSize = 0.0f;
	
	private bool bHorizontal;
	private int iButtonSpacing;
	
	// Use this for initialization
	private void Start()
	{
		
	}
	
	// Update is called once per frame
	private void Update()
	{
		
	}
	
	private void OnGUI()
	{
		if(bShowDialog && conversation.Length > 0) //Make sure we have valid data
		{
			float fAspectRatioWidth = Screen.width / 1024.0f;
			float fAspectRatioHeight = Screen.height / 768.0f;
			
			GUIStyle style = new GUIStyle();
			GUIContent content = new GUIContent();
			Vector2 vSize;
			
			//Label
			style.fontSize = (int)(48 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
			style.fontStyle = FontStyle.Bold;
			style.normal.textColor = new Color(1, 1, 1, 1); //White text
			
			content.text = conversation[iDialogIndex].sActorTalk;
			
			vSize = style.CalcSize(content);
			
			GUI.Label(new Rect((512 * fAspectRatioWidth) - (vSize.x / 2), 100 * fAspectRatioHeight, vSize.x, vSize.y), content, style);
			
			//Button
			style = new GUIStyle(GUI.skin.GetStyle("button"));
			style.fontSize = (int)(48 * (fAspectRatioWidth + fAspectRatioHeight) / 2); //Temporary until we have correct button style
			style.fontStyle = FontStyle.Bold;
			
			//Precalculate space needed
			for(int i = 0; i < conversation[iDialogIndex].answer_choices.Length; i++)
			{
				content.text = conversation[iDialogIndex].answer_choices[i].sPlayerTalk;
				
				vSize = style.CalcSize(content);
				fTotalAnswerSize += bHorizontal ? (iButtonSpacing * fAspectRatioWidth) + vSize.x : (iButtonSpacing * fAspectRatioHeight) + vSize.y;
			}
			
			fTotalAnswerSize -= iButtonSpacing; //Account for last button
			
			for(int i = 0; i < conversation[iDialogIndex].answer_choices.Length; i++)
			{
				content.text = conversation[iDialogIndex].answer_choices[i].sPlayerTalk;
				
				vSize = style.CalcSize(content);
				
				if(GUI.Button(new Rect(bHorizontal ? (512 * fAspectRatioWidth) - (fTotalAnswerSize / 2) + fCurrentOffset : (512 * fAspectRatioWidth) - (vSize.x / 2), bHorizontal ? (384 * fAspectRatioHeight) - (vSize.y / 2) : (384 * fAspectRatioHeight) - (fTotalAnswerSize / 2) + fCurrentOffset, vSize.x, vSize.y), content, style)) //Always center all buttons on screen
				{
					//Log everything the actor or player is saying
					DiaLog.get().log(sName, conversation[iDialogIndex].sActorTalk, conversation[iDialogIndex].iSolutionTag);
					DiaLog.get().log("Player", conversation[iDialogIndex].answer_choices[i].sPlayerTalk);
					
					if(conversation[iDialogIndex].answer_choices[i].iTarget == -1)
					{
						this.transform.GetComponent<CreateInterface>().destroyDialog(); //Quit dialog
					}
					
					if(conversation[iDialogIndex].answer_choices[i].iTarget != -1) //Don't change target if command is quit
					{
						iDialogIndex = conversation[iDialogIndex].answer_choices[i].iTarget;
					}
				}
				
				fCurrentOffset += bHorizontal ? (iButtonSpacing * fAspectRatioWidth) + vSize.x : (iButtonSpacing * fAspectRatioHeight) + vSize.y;
			}
			
			fCurrentOffset = 0.0f;
			fTotalAnswerSize = 0.0f;
		}
	}
	
	public void setSettings(bool _bAlign, int _iSpacing)
	{
		bHorizontal = _bAlign;
		iButtonSpacing = _iSpacing;
	}
	
	public void setDialog(string _name, dialog[] _input)
	{
		sName = _name;
		conversation = _input;
	}
	
	public void show()
	{
		DiaLog.get().newConversation();
		
		bShowDialog = true;
		iDialogIndex = 0;
	}
	
	public void hide()
	{
		bShowDialog = false;
	}
}