using UnityEngine;
using System.Collections;

[System.Serializable]
public class answer
{
	public string sPlayerTalk;
	public int iTarget;
}

[System.Serializable]
public class dialog
{
	public string sActorTalk;
	public int iSolutionTag = -1;
	public answer[] answer_choices;
}

public class ConversationContainer : MonoBehaviour
{
	public bool bConversationEnabled = true;
	public string sName;
	public dialog[] conversation;
	
	// Use this for initialization
	private void Start() 
	{
	
	}
	
	// Update is called once per frame
	private void Update()
	{
	
	}
}