using UnityEngine;
using System.Collections;

public class Timer : MonoBehaviour
{
	public int iCurrentStage = 1;
	public int iMaxStages = 3;
	public int iMaxTime = 30;
	private const int Static = 8;
	private int iSeconds = 0;
	public bool bTimerStopped = false;
	
	// Use this for initialization
	private void Start() 
	{
		updateViableObjects();
	}
	
	// Update is called once per frame
	private void Update()
	{
		if(iCurrentStage < iMaxStages && !bTimerStopped)
		{
			StartCoroutine(updateTime());
		}
	}
	
	private void OnGUI()
	{
		int time = iMaxTime - iSeconds;
		
		int hours = time / 3600;
		int minutes = (time - (hours * 3600)) / 60;
		int seconds = (time - (hours * 3600) - (minutes * 60));
		
		GUI.Label(new Rect(10, 5, 250, 100), hours.ToString("00") + ":" + minutes.ToString("00") + ":" + seconds.ToString("00"));
	}
	
	public void updateViableObjects()
	{
		GameObject[] temp = GameObject.FindObjectsOfType(typeof(GameObject)) as GameObject[];
		
		foreach(GameObject _gameobject in temp)
		{
			if(_gameobject.layer == Static)
			{
				bool isViable = false;
				for(int i = 0; i < _gameobject.GetComponent<Stage>().iStages.Count; i++)
				{
					if(_gameobject.GetComponent<Stage>().iStages[i] == iCurrentStage)
					{
						isViable = true;
						break;
					}
				}
				
				if(!isViable)
				{
					_gameobject.renderer.enabled = false;
					_gameobject.collider.enabled = false;
				}
				else
				{
					_gameobject.renderer.enabled = true;
					_gameobject.collider.enabled = true;
				}
			}
        }
	}
	
	private IEnumerator updateTime()
	{
		yield return new WaitForSeconds(1);
		iSeconds++;
		
		if(iMaxTime - iSeconds <= 0 && !GameObject.FindGameObjectWithTag("Player").GetComponent<CreateDialog>().bShowDialog)
		{
			iCurrentStage++;
			updateViableObjects();
			
			if(iCurrentStage < iMaxStages)
			{
				iSeconds = 0;
			}
		}
		
		StopAllCoroutines();
	}
}