class VBE_SkeletalMesh extends VeiledBoidExtension;

var() const private SkeletalMeshComponent Mesh;

var() const private array<name> IdleAnims;
var() const private array<name> MoveAnims;

var() const private float SpeedAlignedWithAnimation;

var private AnimNodeSequence Node;
var private VBE_SnapToGround GroundedExtension;

function Constructor()
{
	local int i;

	AttachComponent(Mesh);

	GetAnimNode();
	StartMoveAnim();

	for (i = 0; i < Extensions.Length; ++i)
	{
		if (VBE_SnapToGround(Extensions[i]) != none)
		{
			GroundedExtension = VBE_SnapToGround(Extensions[i]);
			break;
		}
	}
}

function Update(float DeltaTime)
{
	if (NeedsControllerUpdate())
	{
		Node.Rate = 1.0f + VSize(Velocity) / SpeedAlignedWithAnimation;
	}
}

function TimerUpdate()
{
	if (GroundedExtension == none || GroundedExtension.bGrounded)
	{
		StartIdleAnim();
	}
}

function private GetAnimNode()
{
	local AnimTree Tree;

	Tree = AnimTree(Mesh.Animations);

	if (Tree != none)
	{
		Node = AnimNodeSequence(Tree.Children[0].Anim);
	}

	if (Node == none)
	{
		Node = AnimNodeSequence(Mesh.Animations);
	}
}

function private StartIdleAnim()
{
	local float AnimLength;
	local int i;

	if (IdleAnims.Length == 0)
	{
		StartMoveAnim();
		return;
	}

	i = Rand(IdleAnims.Length);
	Node.SetAnim(IdleAnims[i]);
	Node.PlayAnim(false, 1.0f, 0.0f);

	AnimLength = Node.GetAnimPlaybackLength();

	if (AnimLength > 0.0f)
	{
		bNeedsControllerUpdate = false;
		SetTimer(AnimLength,, 'Reset', self);
	}
	else
	{
		StartMoveAnim();
	}
}

function private StartMoveAnim()
{
	local int i;

	if (MoveAnims.Length == 0)
	{
		return;
	}

	i = Rand(MoveAnims.Length);
	Node.SetAnim(MoveAnims[i]);
	Node.PlayAnim(true, 1.0f, 0.0f);
}

function private Reset()
{
	StartMoveAnim();

	bNeedsControllerUpdate = true;
}

DefaultProperties
{
	IsUpdate = true;
	IsTimer = true;

	Begin Object class=SkeletalMeshComponent name=Temp
	End Object

	Mesh = Temp;

	SpeedAlignedWithAnimation = 600.0f;
}