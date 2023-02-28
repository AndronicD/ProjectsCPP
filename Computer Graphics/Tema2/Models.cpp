#include "Models.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

#define distInt 0.2f
#define distExt 0.2f
#define distlane 0.1f
#define distCopaci 0.3f
#define pas 1.f
#define tol 0.2f
#define numar_puncte 37

#define inf_lungime -45
#define sup_lungime 30
#define inf_latime -15
#define sup_latime 45
#define lungime_unit 0.2
#define latime_unit 0.2


using namespace std;

float get_angle(glm::vec3 vect) {
	
	float angle = atan2(vect.x, vect.z);
	return angle;
}


Mesh* Models::CreateGrass(const std::string& name) {
	glm::vec3 green = glm::vec3(0.196078, 0.8, 0.19607);
	std::vector<VertexFormat> vertices;

	Mesh* body = new Mesh(name);
	std::vector<unsigned int> indices;
	int count = 0;
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {
			vertices.push_back(VertexFormat(glm::vec3(inf_lungime + j * lungime_unit, -0.07, sup_latime - i * latime_unit), green));
			vertices.push_back(VertexFormat(glm::vec3(inf_lungime + j * lungime_unit, -0.07, sup_latime - (i + 1) * latime_unit), green));
			vertices.push_back(VertexFormat(glm::vec3(inf_lungime + (j + 1) * lungime_unit, -0.07, sup_latime - i * latime_unit), green));
			vertices.push_back(VertexFormat(glm::vec3(inf_lungime + (j + 1) * lungime_unit, -0.07, sup_latime - (i + 1) * latime_unit), green));

			indices.push_back(count);
			indices.push_back(count + 1);
			indices.push_back(count + 3);
			indices.push_back(count);
			indices.push_back(count + 2);
			indices.push_back(count + 3);
			count += 4;
		}
	}

	body->InitFromData(vertices, indices);
	return body;
}


Mesh* Models::CreateRoad(const std::string& name, TPoint *p, TPoint* inside, TPoint* outside, TPoint* lane1, TPoint* lane2, TPoint* rand1, TPoint* rand2) {
	
	glm::vec3 exterior[numar_puncte];
	glm::vec3 interior[numar_puncte];
	glm::vec3 lane_1[numar_puncte];
	glm::vec3 lane_2[numar_puncte];
	glm::vec3 rand1_copaci[numar_puncte];
	glm::vec3 rand2_copaci[numar_puncte];
	glm::vec3 grey = glm::vec3(0.5, 0.5, 0.5);
	
	for (int i = 0; i < numar_puncte; ++i) {
		
		glm::vec3 P1 = glm::vec3(p[i].x, 0, p[i].z);
		P1 *= 3;
		glm::vec3 P2;
		if (i == numar_puncte - 1) {
			P2 = glm::vec3(p[0].x, 0, p[0].z);
		}
		else {
			P2 = glm::vec3(p[i + 1].x, 0, p[i + 1].z);
		}
		P2 *= 3;
		glm::vec3 D = glm::vec3(P2.x - P1.x, 0, P2.z - P1.z);
		glm::vec3 UP = glm::vec3(0, 1, 0);
		glm::vec3 P = glm::cross(D, UP);
		
	
		interior[i] = P1 - distInt * P;
		lane_1[i] = P1 - distlane* P;
		exterior[i] = P1 + distExt * P;
		lane_2[i] = P1 + distlane * P;
		rand1_copaci[i] = P1 + distCopaci * P;;
		rand2_copaci[i] = P1 - distCopaci * P;;
	}

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	int contor = 0;

	for (int i = 0; i < 37 ; ++i) {
		float In_x = interior[i].x;
		float In_z = interior[i].z;
		float Ext_x = exterior[i].x;
		float Ext_z = exterior[i].z;

		glm::vec3 P1 = glm::vec3(p[i].x, 0, p[i].z);
		glm::vec3 P2 = glm::vec3(p[i + 1].x, 0, p[i + 1].z);
		if (i == numar_puncte - 1) {
			P2 = glm::vec3(p[0].x, 0, p[0].z);
		}
		glm::vec3 D = glm::vec3(P2.x - P1.x, 0, P2.z - P1.z);

		float len = glm::length(D);
		float add_x = pas * (D.x/len);
		float add_z = pas * (D.z/len);

		glm::vec3 i1 = glm::vec3(In_x, 0, In_z);
		glm::vec3 e1 = glm::vec3(Ext_x, 0, Ext_z);

		glm::vec3 i2;
		glm::vec3 e2;

		glm::vec3 i2_f;
		glm::vec3 e2_f;


		if (i < numar_puncte - 1) {
			i2 = glm::vec3(interior[i + 1].x, 0, interior[i + 1].z);
			e2 = glm::vec3(exterior[i + 1].x, 0, exterior[i + 1].z);
			i2_f = glm::vec3(interior[i + 1].x, 0, interior[i + 1].z);
			e2_f = glm::vec3(exterior[i + 1].x, 0, exterior[i + 1].z);
		}
		else if (i == numar_puncte - 1) {
			i2 = glm::vec3(interior[0].x, 0, interior[0].z);
			e2 = glm::vec3(exterior[0].x, 0, exterior[0].z);
			i2_f = glm::vec3(interior[0].x, 0, interior[0].z);
			e2_f = glm::vec3(exterior[0].x, 0, exterior[i].z);
		}
		
		float lungime_int = glm::length(i2 - i1);
		glm::vec3 vec_int_step = (i2 - i1);
		vec_int_step.x = vec_int_step.x / 100;
		vec_int_step.z = vec_int_step.z / 100;

		float lungime_ext = glm::length(e2 - e1);
		glm::vec3 vec_ext_step = (e2 - e1);
		vec_ext_step.x = vec_ext_step.x / 100;
		vec_ext_step.z = vec_ext_step.z / 100;

		for (int i = 0; i < 100; i++) {
			i2 = i1 + vec_int_step;
			e2 = e1 + vec_ext_step;

			vertices.push_back(VertexFormat(i1, grey));
			vertices.push_back(VertexFormat(i2, grey));
			vertices.push_back(VertexFormat(e1, grey));
			vertices.push_back(VertexFormat(e2, grey));

			indices.push_back(contor);
			indices.push_back(contor + 2);
			indices.push_back(contor + 3);
			indices.push_back(contor);
			indices.push_back(contor + 1);
			indices.push_back(contor + 3);

			i1 = i1 + vec_int_step;
			e1 = e1 + vec_ext_step;

			contor += 4;
		}

		if (lungime_int > lungime_ext) {
			vertices.push_back(VertexFormat(i2, grey));
			vertices.push_back(VertexFormat(e2, grey));
			vertices.push_back(VertexFormat(i2_f, grey));

			indices.push_back(contor);
			indices.push_back(contor + 1);
			indices.push_back(contor + 2);

			contor += 3;
		}
		else {
			vertices.push_back(VertexFormat(i2, grey));
			vertices.push_back(VertexFormat(e2, grey));
			vertices.push_back(VertexFormat(e2_f, grey));

			indices.push_back(contor);
			indices.push_back(contor + 1);
			indices.push_back(contor + 2);

			contor += 3;
		}
		
	}

	for (int i = 0; i < 37; i++) {
		inside[i].x = interior[i].x;
		inside[i].z = interior[i].z;
		outside[i].x = exterior[i].x;
		outside[i].z = exterior[i].z;
		lane1[i].x = lane_1[i].x;
		lane1[i].z = lane_1[i].z;
		lane2[i].x = lane_2[i].x;
		lane2[i].z = lane_2[i].z;
		rand1[i].x = rand1_copaci[i].x;
		rand1[i].z = rand1_copaci[i].z;
		rand2[i].x = rand2_copaci[i].x;
		rand2[i].z = rand2_copaci[i].z;
	}

	Mesh* body = new Mesh(name);
	body->InitFromData(vertices, indices);
	return body;
}

Mesh* Models::CreateTree(const std::string& name) {
	glm::vec3 dark_green = glm::vec3(0.184314, 0.309804, 0.184314);
	glm::vec3 brown = glm::vec3(0.647059, 0.164706, 0.164706);
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, -0.5, 0.2), brown),
		VertexFormat(glm::vec3(0.2, -0.5, 0.2), brown),
		VertexFormat(glm::vec3(0, 0.4, 0.2), brown),
		VertexFormat(glm::vec3(0.2, 0.4, 0.2), brown),
		VertexFormat(glm::vec3(0, -0.5, 0), brown),
		VertexFormat(glm::vec3(0.2, -0.5, 0), brown),
		VertexFormat(glm::vec3(0, 0.4, 0), brown),
		VertexFormat(glm::vec3(0.2, 0.4, 0), brown), //trunchi

		VertexFormat(glm::vec3(0.4, 0.4, -0.2), dark_green),
		VertexFormat(glm::vec3(0.4, 0.4, 0.4), dark_green),
		VertexFormat(glm::vec3(0.4, 0.8, -0.2), dark_green),
		VertexFormat(glm::vec3(0.4, 0.8, 0.4), dark_green),
		VertexFormat(glm::vec3(-0.2, 0.4, -0.2), dark_green),
		VertexFormat(glm::vec3(-0.2, 0.4, 0.4), dark_green),
		VertexFormat(glm::vec3(-0.2, 0.8, -0.2), dark_green),
		VertexFormat(glm::vec3(-0.2, 0.8, 0.4), dark_green), //frunze
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		8, 9, 10,
		9, 11, 10,
		10, 11, 15,
		10, 15, 14,
		9, 15, 11,
		9, 13, 15,
		14, 15, 12,
		15, 13, 12,
		8, 12, 9,
		9, 12, 13,
		10, 14, 12,
		8, 10, 12,

	};
	Mesh* body = new Mesh(name);
	body->InitFromData(vertices, indices);
	return body;
}

Mesh* Models::CreateCar(const std::string& name, const glm::vec3 color) {
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-0.1, 0, 0.1), color),
		VertexFormat(glm::vec3(0.1, 0, 0.1), color),
		VertexFormat(glm::vec3(-0.1, 0.2, 0.1), color),
		VertexFormat(glm::vec3(0.1, 0.2, 0.1), color),
		VertexFormat(glm::vec3(-0.1, 0, -0.1), color),
		VertexFormat(glm::vec3(0.1, 0, -0.1), color),
		VertexFormat(glm::vec3(-0.1, 0.2, -0.1), color),
		VertexFormat(glm::vec3(0.1, 0.2, -0.1), color),
	};
	vector<unsigned int> indices =
	{
		0, 1, 2,    
		1, 3, 2,    
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

	};
	Mesh* body = new Mesh(name);
	body->InitFromData(vertices, indices);
	return body;
}