using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class solution_piece
{
	public string sText;
	public Texture2D texImage;
	public bool bTextEnabled;
	public Color colorActive;
	public int iSolutionTag;
	public bool bSolved;
}

public class Solution : MonoBehaviour
{
	private static Solution singleton;
	private List<list_string> sLabelTexts;
	private List<list_ints> iCorrectSolutionIDs;
	private List<List<solution_piece>> solutionList = new List<List<solution_piece>>();
	private Color colorStandard = GUI.backgroundColor;
	private int iCurrentMurder;
	private int iPreviousMurder = 0;
	public bool bSolutionEnabled = false;
	
	// Use this for initialization
	private void Start() 
	{
		if(singleton == null)
		{
			singleton = this;
		}
		
		//Set labels to specific names
		sLabelTexts = this.transform.gameObject.GetComponent<EnableAction>().sCorrectSolutionLabels;
		
		//Fill list with correct solution data
		iCorrectSolutionIDs = this.transform.gameObject.GetComponent<EnableAction>().iCorrectSolutionIDs;
		
		for(int i = 0; i < iCorrectSolutionIDs.Count; i++) //Count possible number of solution windows
		{
			solutionList.Add(new List<solution_piece>());
			
			for(int j = 0; j < iCorrectSolutionIDs[i].Count; j++)
			{
				solutionList[solutionList.Count - 1].Add(new solution_piece());
				solutionList[solutionList.Count - 1][solutionList[solutionList.Count - 1].Count - 1].bTextEnabled = true;
				solutionList[solutionList.Count - 1][solutionList[solutionList.Count - 1].Count - 1].sText = "";
				solutionList[solutionList.Count - 1][solutionList[solutionList.Count - 1].Count - 1].colorActive = colorStandard;
				solutionList[solutionList.Count - 1][solutionList[solutionList.Count - 1].Count - 1].iSolutionTag = -1;
				solutionList[solutionList.Count - 1][solutionList[solutionList.Count - 1].Count - 1].bSolved = false;
			}
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
		
		GUI.Box(new Rect(306 * fAspectRatioWidth, 499.5f * fAspectRatioHeight, 643 * fAspectRatioWidth, 193.5f * fAspectRatioHeight), "Solution");
		
		iCurrentMurder = (int)GUI.VerticalSlider(new Rect(932.5f * fAspectRatioWidth, 525 * fAspectRatioHeight, 10, 150 * fAspectRatioHeight), iCurrentMurder, 0, solutionList.Count - 1);
		
		if(iCurrentMurder != iPreviousMurder) //Deselect all solution windows
		{
			for(int i = 0; i < solutionList[iPreviousMurder].Count; i++)
			{
				if(!solutionList[iPreviousMurder][i].bSolved)
				{
					solutionList[iPreviousMurder][i].colorActive = colorStandard;
				}
			}
			
			bSolutionEnabled = false;
			iPreviousMurder = iCurrentMurder;
		}
		
		if(GUI.Button(new Rect(325 * fAspectRatioWidth, 500 * fAspectRatioHeight, 85 * fAspectRatioWidth, 25 * fAspectRatioHeight), "Solve murder!"))
		{
			int iCorrect = 0;
			
			for(int i = 0; i < solutionList[iCurrentMurder].Count; i++)
			{
				if(solutionList[iCurrentMurder][i].iSolutionTag == iCorrectSolutionIDs[iCurrentMurder][i])
				{
					iCorrect++;
				}
			}
			
			if(iCorrect == solutionList[iCurrentMurder].Count)
			{
				bSolutionEnabled = false;
				
				for(int i = 0; i < solutionList[iCurrentMurder].Count; i++)
				{
					solutionList[iCurrentMurder][i].colorActive = Color.green;
					solutionList[iCurrentMurder][i].bSolved = true;
				}
			}
		}
		
		for(int i = 0; i < solutionList[iCurrentMurder].Count; i++)
		{
			GUI.Label(new Rect(325 * fAspectRatioWidth + i * (625 / solutionList[iCurrentMurder].Count * fAspectRatioWidth), 525 * fAspectRatioHeight, 500 / solutionList[iCurrentMurder].Count * fAspectRatioWidth, 130 * fAspectRatioHeight), sLabelTexts[iCurrentMurder][i]);
			
			GUI.backgroundColor = solutionList[iCurrentMurder][i].colorActive;
			
			if(solutionList[iCurrentMurder][i].bTextEnabled)
			{
				if(GUI.Button(new Rect(325 * fAspectRatioWidth + i * (625 / solutionList[iCurrentMurder].Count * fAspectRatioWidth), 550 * fAspectRatioHeight, 500 / solutionList[iCurrentMurder].Count * fAspectRatioWidth, 130 * fAspectRatioHeight), solutionList[iCurrentMurder][i].sText))
				{
					if(solutionList[iCurrentMurder][i].colorActive != colorStandard && !solutionList[iCurrentMurder][i].bSolved) //Only allow changing if murder hasn't been solved already
					{
						bSolutionEnabled = false;
						solutionList[iCurrentMurder][i].colorActive = colorStandard;
					}
					else if(!solutionList[iCurrentMurder][i].bSolved)
					{
						bSolutionEnabled = true;
						
						for(int j = 0; j < solutionList[iCurrentMurder].Count; j++)
						{
							solutionList[iCurrentMurder][j].colorActive = colorStandard;
						}
						
						solutionList[iCurrentMurder][i].colorActive = Color.red;
					}
				}
			}
			else
			{
				if(GUI.Button(new Rect(325 * fAspectRatioWidth + i * (625 / solutionList[iCurrentMurder].Count * fAspectRatioWidth), 550 * fAspectRatioHeight, 500 / solutionList[iCurrentMurder].Count * fAspectRatioWidth, 130 * fAspectRatioHeight), solutionList[iCurrentMurder][i].texImage))
				{
					if(solutionList[iCurrentMurder][i].colorActive != colorStandard && !solutionList[iCurrentMurder][i].bSolved)
					{
						bSolutionEnabled = false;
						solutionList[iCurrentMurder][i].colorActive = colorStandard;
					}
					else if(!solutionList[iCurrentMurder][i].bSolved)
					{
						bSolutionEnabled = true;
						
						for(int j = 0; j < solutionList.Count; j++)
						{
							solutionList[iCurrentMurder][j].colorActive = colorStandard;
						}
						
						solutionList[iCurrentMurder][i].colorActive = Color.red;
					}
				}
			}
		}
		
		//Everything solved
		bool bSolved = true;
		for(int i = 0; i < solutionList.Count; i++)
		{
			for(int j = 0; j < solutionList[i].Count; j++)
			{
				if(!solutionList[i][j].bSolved)
				{
					bSolved = false;
				}
			}
		}
		
		GameObject player = GameObject.FindGameObjectWithTag("Player");
		if(bSolved && player.GetComponent<Timer>().iCurrentStage != player.GetComponent<Timer>().iMaxStages - 1)
		{
			player.GetComponent<Timer>().bTimerStopped = true;
			player.GetComponent<Timer>().iCurrentStage = 0;
			player.GetComponent<Timer>().updateViableObjects();
		}
	}
	
	public void insertDialog(string _info, int _id)
	{
		for(int i = 0; i < solutionList[iCurrentMurder].Count; i++)
		{
			if(solutionList[iCurrentMurder][i].colorActive != colorStandard)
			{
				solutionList[iCurrentMurder][i].sText = _info;
				solutionList[iCurrentMurder][i].bTextEnabled = true;
				solutionList[iCurrentMurder][i].iSolutionTag = _id;
			}
		}
	}
	
	public void insertItem(string _info, Texture2D _image, int _id)
	{
		for(int i = 0; i < solutionList[iCurrentMurder].Count; i++)
		{
			if(solutionList[iCurrentMurder][i].colorActive != colorStandard)
			{
				solutionList[iCurrentMurder][i].sText = _info;
				solutionList[iCurrentMurder][i].texImage = _image;
				solutionList[iCurrentMurder][i].bTextEnabled = false;
				solutionList[iCurrentMurder][i].iSolutionTag = _id;
			}
		}
	}
	
	public static Solution get()
	{
		if(singleton != null)
		{
			return singleton;
		}
		
		return null;
	}
}