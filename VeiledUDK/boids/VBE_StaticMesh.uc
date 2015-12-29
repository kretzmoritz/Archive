class VBE_StaticMesh extends VeiledBoidExtension;

var() const private StaticMeshComponent Mesh;

function Constructor()
{
	AttachComponent(Mesh);
}

DefaultProperties
{
	Begin Object class=StaticMeshComponent name=Temp
	End Object

	Mesh = Temp;
}