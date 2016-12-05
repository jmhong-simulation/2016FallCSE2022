/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GLObject.h"

class GLLineSegments : public GLObject
{
public:
	using GLObject::vertices;
	using GLObject::vertexbuffer;
	using GLObject::genVertexBuffer;

	void update(const std::vector<glm::vec3>& vertices_input)
	{
		vertices.clear();

		for(auto itr : vertices_input)
		{
			vertices.push_back(itr);
		}

		genVertexBuffer();
	}
};
