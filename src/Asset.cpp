#include "Precompiled.h"
#include "Asset.h"
#include "Node.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Text.h"
#include "PolyLine.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Guid.h"

bool Asset::GetVec2(json & node, const string & name, Vec2f & v) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_array())
			{
				if (record->size() >= 2)
				{
					v.x = record->at(0);
					v.y = record->at(1);

					return true;
				}
				else
				{
					Log() << "Vector must be array of two numbers!" << LineEnd;
				}
			}
			else
			{
				Log() << name << " must be array!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}

bool Asset::GetString(json & node, const string & name, string & s) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_string())
			{
				string temp = *record;

				s = temp;

				return true;
			}
			else
			{
				Log() << name << " must be string!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}

bool Asset::GetBoolean(json & node, const string & name, bool & b) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_string())
			{
				b = *record;

				return true;
			}
			else
			{
				Log() << name << " must be boolean!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}

bool Asset::GetColor(json & node, const string & name, sf::Color & clr) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_array())
			{
				if (record->size() >= 4)
				{
					clr.r = record->at(0);
					clr.g = record->at(1);
					clr.b = record->at(2);
					clr.a = record->at(3);

					return true;
				}
				else
				{
					Log() << "Color must be array of four numbers!" << LineEnd;
				}
			}
			else
			{
				Log() << name << " must be array!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}

Asset::Asset()
{
	mRoot["scenes"] = json();
}

Asset::~Asset()
{

}

void Asset::AddScene(const shared_ptr<Scene>& scene)
{
	json root;

	json nodes;
	for (auto & node : scene->GetNodeList())
	{
		json nodeDesc;

		if (auto sprite = dynamic_pointer_cast<Sprite>(node))
		{
			nodeDesc["type"] = "sprite";
			nodeDesc["flipX"] = sprite->IsXFlipped();
			nodeDesc["flipY"] = sprite->IsYFlipped();
			nodeDesc["renderOrder"] = sprite->GetRenderOrder();
			sf::Color color = sprite->GetColor();
			nodeDesc["color"] = { color.r, color.g, color.b, color.a };
			sf::FloatRect textureRect = sprite->GetTextureRectangle();
			nodeDesc["textureRect"] = { textureRect.left, textureRect.top, textureRect.width, textureRect.height };
			sf::FloatRect decalTextureRect = sprite->GetDecalTextureRectangle();
			nodeDesc["decalTextureRect"] = { decalTextureRect.left, decalTextureRect.top, decalTextureRect.width, decalTextureRect.height };
			if (auto texture = sprite->GetTexture())
			{
				nodeDesc["texture"] = texture->GetSource();
			}
		}
		else if (auto light = dynamic_pointer_cast<Light>(node))
		{
			nodeDesc["type"] = "light";
			nodeDesc["radius"] = light->GetRadius();
			sf::Color color = light->GetColor();
			nodeDesc["color"] = { color.r, color.g, color.b, color.a };
			nodeDesc["radiusJitter"] = light->GetRadiusJitter();
		}
		else if (auto particleSystem = dynamic_pointer_cast<ParticleSystem>(node))
		{
			nodeDesc["type"] = "particlesystem";
			nodeDesc["particleCount"] = particleSystem->GetParticleCount();
			switch (particleSystem->GetBoundsType())
			{
			case ParticleSystem::BoundsType::Circle:
				nodeDesc["boundsType"] = "circle";
				break;
			case ParticleSystem::BoundsType::Rect:
				nodeDesc["boundsType"] = "rect";
				break;
			}
			if (auto texture = particleSystem->GetTexture())
			{
				nodeDesc["texture"] = texture->GetSource();
			}
			Vec2f speedMin = particleSystem->GetMinSpeed();
			nodeDesc["speedMin"] = { speedMin.x, speedMin.y };
			Vec2f speedMax = particleSystem->GetMinSpeed();
			nodeDesc["speedMax"] = { speedMax.x, speedMax.y };
			sf::Color beginColor = particleSystem->GetBeginColor();
			nodeDesc["beginColor"] = { beginColor.r, beginColor.g, beginColor.b, beginColor.a };
			sf::Color endColor = particleSystem->GetEndColor();
			nodeDesc["endColor"] = { endColor.r, endColor.g, endColor.b, endColor.a };
			nodeDesc["particleSize"] = particleSystem->GetParticleSize();
			nodeDesc["radius"] = particleSystem->GetRadius();
			sf::FloatRect rect = particleSystem->GetRectangle();
			nodeDesc["rect"] = { rect.left, rect.top, rect.width, rect.height };

		}
		else if (auto text = dynamic_pointer_cast<Text>(node))
		{
			nodeDesc["type"] = "text";
		}
		else if (auto polyline = dynamic_pointer_cast<PolyLine>(node))
		{
			nodeDesc["type"] = "polyline";
			json points;
			for (auto & pt : polyline->GetPoints())
			{
				json point;
				point.push_back(pt.x);
				point.push_back(pt.y);
				points.push_back(point);
			}
			nodeDesc["points"] = points;
			sf::Color color = polyline->GetColor();
			nodeDesc["color"] = { color.r, color.g, color.b, color.a };
			nodeDesc["thickness"] = polyline->GetThickness();
		}
		else
		{
			nodeDesc["type"] = "node";
		}
		nodeDesc["name"] = node->GetName();
		nodeDesc["size"] = { node->GetSize().x, node->GetSize().y };
		nodeDesc["position"] = { node->GetLocalPosition().x, node->GetLocalPosition().y };
		nodeDesc["origin"] = { node->GetLocalOrigin().x, node->GetLocalOrigin().y };
		nodeDesc["scale"] = { node->GetLocalScale().x, node->GetLocalScale().y };
		nodeDesc["rotation"] = node->GetLocalRotation();
		nodeDesc["id"] = Guid::Generate().ToString();

		nodes.push_back(nodeDesc);
	}
	root["nodes"] = nodes;

	json animations;
	for (auto & anim : scene->GetAnimationList())
	{
		json animationDesc;
		animationDesc["name"] = anim->GetName();
		animationDesc["enabled"] = anim->IsEnabled();
		animationDesc["looped"] = anim->IsLooped();
		animationDesc["speed"] = anim->GetSpeed();
		animationDesc["length"] = anim->GetLength();
		animationDesc["id"] = Guid::Generate().ToString();

		json tracks;
		for (auto & track : anim->GetTrackList())
		{
			json trackDesc;
			trackDesc["name"] = track->GetName();
			trackDesc["enabled"] = track->IsEnabled();
			trackDesc["id"] = Guid::Generate().ToString();

			json keyframes;
			for (auto & keyframe : track->GetKeyframeList())
			{
				json keyframeDesc;
				keyframeDesc["position"] = { keyframe.GetPosition().x, keyframe.GetPosition().y };
				keyframeDesc["scale"] = { keyframe.GetScale().x, keyframe.GetScale().y };
				keyframeDesc["rotation"] = keyframe.GetRotation();
				keyframeDesc["time"] = keyframe.GetTime();
				keyframeDesc["id"] = Guid::Generate().ToString();
				keyframes.push_back(keyframeDesc);
			}
			trackDesc["keyframes"] = keyframes;

			tracks.push_back(trackDesc);
		}
		animationDesc["tracks"] = tracks;

		animations.push_back(animationDesc);
	}
	root["animations"] = animations;

	json connections;
	int childId = 0;
	for (auto & node : scene->GetNodeList())
	{
		json childParentPair;
		childParentPair.push_back(childId++);
		if (node->HasParent())
		{
			auto parent = node->GetParent();
			int parentId = 0;
			for (auto & otherNode : scene->GetNodeList())
			{
				if (otherNode == parent)
				{
					break;
				}
				++parentId;
			}
			childParentPair.push_back(parentId);
		}
		else
		{
			childParentPair.push_back(-1);
		}
		connections.push_back(childParentPair);
	}
	root["nodeConnections"] = connections;

	mRoot["scenes"].push_back(root);
}

void Asset::Save(const string & path)
{
	std::ofstream of(path);
	of << setw(2) << mRoot << endl;
}

// Loading is flexible: node will not be constructed only if something critical happened
void Asset::Load(const string & path)
{
	shared_ptr<Scene> scene;

	try
	{
		std::ifstream i(path);
		json root;
		i >> root;

		json nodes;
		nodes = root.at("nodes");

		scene = game->CreateScene();

		for (auto & nodeDesc : nodes)
		{
			string type;
			if (GetString(nodeDesc, "type", type))
			{
				shared_ptr<SceneNode> node;
				if (type == "node")
				{
					node = scene->CreateNode();
				}
				else if (type == "sprite")
				{
					shared_ptr<Sprite> sprite = scene->CreateSprite();

					bool flipX;
					if (GetBoolean(nodeDesc, "flipX", flipX))
					{
						sprite->FlipX(flipX);
					}

					bool flipY;
					if (GetBoolean(nodeDesc, "flipY", flipY))
					{
						sprite->FlipY(flipY);
					}

					int renderOrder;
					if (GetNumber(nodeDesc, "renderOrder", renderOrder))
					{

					}

					sf::Color color;
					if (GetColor(nodeDesc, "color", color))
					{
						sprite->SetColor(color);
					}

					sf::FloatRect textureRect;
					if (GetRect(nodeDesc, "textureRect", textureRect))
					{
						sprite->SetTextureRectangle(textureRect);
					}

					sf::FloatRect decalTextureRect;
					if (GetRect(nodeDesc, "decalTextureRect", decalTextureRect))
					{
						sprite->SetDecalTextureRectangle(decalTextureRect);
					}

					string texture;
					if (GetString(nodeDesc, "texture", texture))
					{
						sprite->SetTexture(game->GetRenderer()->RequestTexture(texture));
					}

					node = sprite;
				}
				else if (type == "polyline")
				{
					shared_ptr<PolyLine> polyline = scene->CreatePolyLine();


					node = polyline;
				}
				else if (type == "text")
				{
					node = scene->CreateText();
				}
				else if (type == "particlesystem")
				{
					node = scene->CreateParticleSystem();
				}
				else
				{
					Log() << "Unknown node type was detected - " << type << ". Nothing was created" << LineEnd;
				}

				// Set generic properties
				if (node)
				{
					Vec2f position;
					if (GetVec2(nodeDesc, "position", position))
					{
						node->SetLocalPosition(position);
					}

					Vec2f origin;
					if (GetVec2(nodeDesc, "origin", origin))
					{
						node->SetLocalOrigin(origin);
					}

					Vec2f size;
					if (GetVec2(nodeDesc, "size", size))
					{
						node->SetSize(size);
					}

					Vec2f scale;
					if (GetVec2(nodeDesc, "scale", scale))
					{
						node->SetLocalScale(scale);
					}

					float rotation;
					if (GetNumber(nodeDesc, "rotation", rotation))
					{
						node->SetLocalRotation(rotation);
					}

					string name;
					if (GetString(nodeDesc, "name", name))
					{
						node->SetName(name);
					}
				}
			}
			else
			{
				Log() << "Ignoring node because of invalid structure" << LineEnd;
			}
		}
	}
	catch (json::exception &e)
	{
		Log() << "Unable to load prefab " << path << ". Reason: " << e.what() << LineEnd;
	}

	mSceneList.push_back(scene);
}
