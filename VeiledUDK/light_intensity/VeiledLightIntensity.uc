class VeiledLightIntensity extends SequenceCondition;

var() Actor obj;

var float intensity;

event Activated()
{
	local Light l;
	local Vector dirToObj;
	local float distToObj;
	local float angleDifference;
	local float atten;
	local float cone_falloff;

	intensity = 0.0f;

	if (Controller(obj) != none)
	{
		obj = Controller(obj).Pawn;
	}

	foreach GetWorldInfo().AllActors(class'Light', l)
	{
		if (!l.LightComponent.bEnabled)
		{
			return;
		}

		if (PointLight(l) != none)
		{
			distToObj = VSize(obj.Location - l.Location);

			if (distToObj <= PointLightComponent(l.LightComponent).Radius)
			{
				if (obj.FastTrace(l.Location))
				{
					atten = (1.0f - (distToObj / PointLightComponent(l.LightComponent).Radius)) ** PointLightComponent(l.LightComponent).FalloffExponent;
					intensity += l.LightComponent.Brightness * atten;
				}
			}
		}
		else if (SpotLight(l) != none)
		{
			dirToObj = obj.Location - l.Location;
			distToObj = VSize(dirToObj);

			if (distToObj <= SpotLightComponent(l.LightComponent).Radius)
			{
				dirToObj = Normal(dirToObj);
				angleDifference = acos(dirToObj Dot l.LightComponent.GetDirection()) * (180.0f / PI);

				if (angleDifference <= SpotLightComponent(l.LightComponent).OuterConeAngle)
				{
					if (obj.FastTrace(l.Location))
					{
						atten = (1.0f - (distToObj / SpotLightComponent(l.LightComponent).Radius)) ** SpotLightComponent(l.LightComponent).FalloffExponent;

						if (angleDifference > SpotLightComponent(l.LightComponent).InnerConeAngle)
						{
							cone_falloff =  1.0f - ((angleDifference - SpotLightComponent(l.LightComponent).InnerConeAngle) /
											(SpotLightComponent(l.LightComponent).OuterConeAngle - SpotLightComponent(l.LightComponent).InnerConeAngle));
						}
						else
						{
							cone_falloff = 1.0f;
						}

						intensity += l.LightComponent.Brightness * atten * cone_falloff;
					}
				}
			}
		}
		else if (DirectionalLight(l) != none)
		{
			if (obj.FastTrace(obj.Location + (-l.LightComponent.GetDirection() * 5000.0f))) //1 unit = 2cm; 5000 units = 100m
			{
				intensity += l.LightComponent.Brightness;
			}
		}
		else if (SkyLight(l) != none)
		{
			intensity += l.LightComponent.Brightness;
		}
	}
}

DefaultProperties
{
	ObjName="LightIntensity"
	ObjCategory="VeiledSensor"

	OutputLinks.Empty

	VariableLinks.Empty
	VariableLinks[0] = (ExpectedType=class'SeqVar_Object', LinkDesc="Object", PropertyName=obj)
	VariableLinks[1] = (ExpectedType=class'SeqVar_Float', LinkDesc="Intensity", PropertyName=intensity)
}