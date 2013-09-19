using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class sentence
{
	public string sWhoSaidIt;
	public string sInformation;
	public int iSolutionTag;
}

public class DiaLog : MonoBehaviour
{
	private static DiaLog singleton;
	private int iLogElement = -1;
	private int iSliderChoice;
	private int iTextCount;
	private List<int> iSliderChoiceText = new List<int>();
	public List<List<sentence>> log_list = new List<List<sentence>>();
	
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
		
	}
	
	public void show()
	{
		float fAspectRatioWidth = Screen.width / 1024.0f;
		float fAspectRatioHeight = Screen.height / 768.0f;
		int iViewPossibleOnScreen = 14;
		
		GUIStyle style = new GUIStyle(GUI.skin.GetStyle("label"));
		GUIContent content = new GUIContent();
		Vector2 vSize;
		
		GUI.Box(new Rect(25 * fAspectRatioWidth, 25 * fAspectRatioHeight, 256 * fAspectRatioWidth, 668 * fAspectRatioHeight), "DiaLog");
		
		if(iLogElement == -1 && log_list.Count > iViewPossibleOnScreen)
		{
			iSliderChoice = (int)GUI.VerticalSlider(new Rect(256 * fAspectRatioWidth, 60 * fAspectRatioHeight, 10, 608 * fAspectRatioHeight), iSliderChoice, 0, log_list.Count - iViewPossibleOnScreen);
		}
		
		float fPosition = 60 * fAspectRatioHeight;
		for(int i = iSliderChoice; i < (log_list.Count <= iViewPossibleOnScreen + iSliderChoice ? log_list.Count : iViewPossibleOnScreen + iSliderChoice); i++)
		{
			if(iLogElement == -1)
			{
				if(GUI.Button(new Rect(35 * fAspectRatioWidth, fPosition, 206 * fAspectRatioWidth, 35 * fAspectRatioHeight), log_list[i][0].sWhoSaidIt))
				{
					iLogElement = i;
				}
			
				fPosition += 45 * fAspectRatioHeight;
			}
			else if(iLogElement == i)
			{
				//Calculate slider length
				iTextCount = -1;
				fPosition += 65 * fAspectRatioHeight;
				
				for(int j = 0; j < log_list[i].Count; j++)
				{
					style.fontSize = (int)(16 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
					
					string sText = log_list[i][j].sWhoSaidIt + ": " + log_list[i][j].sInformation;
					content.text = sText;
					
					vSize = style.CalcSize(content);
					float fPositionOffset = Mathf.Ceil(vSize.x / (206 * fAspectRatioWidth));
					
					fPosition += (20 * fAspectRatioHeight) * fPositionOffset;
					iTextCount++;
					
					if(fPosition > 650 * fAspectRatioHeight)
					{
						break;
					}
				}
				
				//Reset position values
				fPosition = 60 * fAspectRatioHeight;
				
				//Draw slider and back button
				if(log_list[i].Count - iTextCount > 1)
				{
					iSliderChoiceText[i] = (int)GUI.VerticalSlider(new Rect(260 * fAspectRatioWidth, 60 * fAspectRatioHeight, 10, 608 * fAspectRatioHeight), iSliderChoiceText[i], 0, log_list[i].Count - iTextCount);
				}
					
				if(GUI.Button(new Rect(35 * fAspectRatioWidth, fPosition, 206 * fAspectRatioWidth, 35 * fAspectRatioHeight), "Back"))
				{
					iLogElement = -1;
				}
				
				//Draw DiaLog conversation
				fPosition += 65 * fAspectRatioHeight;
				
				int iPlayerTalkID = 0;
				for(int j = iSliderChoiceText[i]; j < log_list[i].Count; j++)
				{
					style.fontSize = (int)(16 * (fAspectRatioWidth + fAspectRatioHeight) / 2);
					
					string sText = log_list[i][j].sWhoSaidIt + ": " + log_list[i][j].sInformation;
					content.text = sText;
					
					vSize = style.CalcSize(content);
					float fPositionOffset = Mathf.Ceil(vSize.x / (206 * fAspectRatioWidth));
					vSize.y *= fPositionOffset;
					
					GUI.Label(new Rect(35 * fAspectRatioWidth, fPosition, 206 * fAspectRatioWidth, vSize.y), content, style);
					
					if(log_list[i][j].sWhoSaidIt != "Player")
					{
						iPlayerTalkID++;
						
						if(GUI.Button(new Rect(225 * fAspectRatioWidth, fPosition, 25 * fAspectRatioWidth, 25 * fAspectRatioHeight), "S"))
						{
							if(Solution.get().bSolutionEnabled)
							{
								Solution.get().insertDialog(log_list[i][j].sWhoSaidIt + ' ' + iPlayerTalkID, log_list[i][j].iSolutionTag);
							}
						}
					}
					
					fPosition += (20 * fAspectRatioHeight) * fPositionOffset;
					
					if(fPosition > 650 * fAspectRatioHeight)
					{
						break;
					}
				}
			}
		}
	}
	
	public void newConversation()
	{
		//Create a new conversation in our log data structure
		log_list.Add(new List<sentence>());
		iSliderChoiceText.Add(new int());
	}
	
	public void log(string _parent, string _talk, int _id = -1)
	{
		log_list[log_list.Count - 1].Add(new sentence());
		log_list[log_list.Count - 1][log_list[log_list.Count - 1].Count - 1].sWhoSaidIt = _parent;
		log_list[log_list.Count - 1][log_list[log_list.Count - 1].Count - 1].sInformation = _talk;
		log_list[log_list.Count - 1][log_list[log_list.Count - 1].Count - 1].iSolutionTag = _id;
	}
	
	public static DiaLog get()
	{
		if(singleton != null)
		{
			return singleton;
		}
		
		return null;
	}
}