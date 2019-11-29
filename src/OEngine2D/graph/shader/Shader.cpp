#include "Shader.h"
#include "SubShader.h"
#include "Renderable.h"
#include "tinyxml/tinyxml.h"
#include "Pass.h"
#include "RenderState.h"
#include "../../system/FileSystem.h"

namespace oengine2d {
	bool Shader::Load() {
		std::string data;
		if (!FileSystem::GetInstance().ReadFile(_path, data)) {
			return false;
		}

		TiXmlDocument doc;
		doc.Parse(data.c_str());
		if (doc.Error()) {
			return false;
		}

		TiXmlElement* properties = doc.FirstChildElement()->FirstChildElement("properties");
		if (properties) {
			for (TiXmlElement* el = properties->FirstChildElement("property"); el != nullptr; el = el->NextSiblingElement("property")) {
				if (!ParseProperty(el))
					return false;
			}
		}

		for (TiXmlElement* el = doc.FirstChildElement()->FirstChildElement("subshader"); el != nullptr; el = el->NextSiblingElement("subshader")) {
			if (!ParseSubshader(el))
				return false;
		}

		return true;
	}

	void Shader::RecordCommand(uint32_t frame, Renderable& object, const PropertyHandler& handler, const std::string& stage, CommandBuffer& cmd) {
		auto* vb = object.GetVertexBuffer();
		if (!vb) {
			//to do: log warning message
			return;
		}

		for (auto* subShader : _subshaders) {
			if (subShader->GetLodNeed() <= object.GetLod()) {
				subShader->RecordCommand(frame, object, handler, stage, cmd);
				return;
			}
		}

		//to do: log warning message
	}

	void Shader::AddSubShader(SubShader* subShader) {
		subShader->SetShader(this);
		_subshaders.emplace_back(subShader);
	}

	bool Shader::ParseProperty(TiXmlElement* el) {
		return true;
	}

	bool Shader::ParseSubshader(TiXmlElement* el) {
		SubShader* subshader = new SubShader(this);
		TiXmlElement* lod = el->FirstChildElement("lod");
		if (lod)
			subshader->SetLodNeed(atoi(lod->Attribute("value")));

		int32_t i = 0;
		for (TiXmlElement* child = el->FirstChildElement("pass"); child != nullptr; child = child->NextSiblingElement("pass")) {
			Pass* pass = ParsePass(child);
			if (!pass) {
				delete subshader;
				return false;
			}
			
			char temp[256];
			SafeSprintf(temp, sizeof(temp), "%s:S%d:P%d", _path.c_str(), (int32_t)_subshaders.size(), i);
			pass->SetName(temp);

			subshader->AddPass(pass);
			++i;
		}

		AddSubShader(subshader);
		return true;
	}

	Pass* Shader::ParsePass(TiXmlElement* el) {
		Pass* pass = new Pass;

		RenderState rs;
		for (TiXmlElement* child = el->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
			if (strcmp(child->Value(), "stage") == 0) {
				pass->SetStage(child->Attribute("value"));
			}
			else if (strcmp(child->Value(), "vs") == 0) {
				pass->SetVertexShader(child->FirstChild()->ToText()->Value());
			}
			else if (strcmp(child->Value(), "fs") == 0) {
				pass->SetFragmentShader(child->FirstChild()->ToText()->Value());
			}
			else {
				if (!ParseRenderState(child, rs)) {
					delete pass;
					return nullptr;
				}
			}
		}

		rs.Build();
		pass->SetRenderState(rs);
		if (!pass->CreatePipeline()) {
			delete pass;
			return nullptr;
		}

		return pass;
	}

	bool Shader::ParseRenderState(TiXmlElement* el, RenderState& state) {
		return true;
	}
}
