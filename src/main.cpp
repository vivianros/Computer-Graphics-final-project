// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"

// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>

// Linear Algebra Library
#include <Eigen/Core>
#include <Eigen/Geometry>


//#include <mmsystem.h>
//#pragma comment(lib, "WINMM.LIB")


// Timer
#include <chrono>

using namespace std;
using namespace Eigen;

// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject VBO_C;
VertexBufferObject VBO_FLOOR;
VertexBufferObject VBO_LINE;
VertexBufferObject VBO_CLINE;
VertexBufferObject VBO_UV;
VertexBufferObject VBO_END;
VertexBufferObject VBO_CEND;


ElementBufferObject EBO;
Eigen::Matrix4f camera_view;
Eigen::MatrixXf V(3,10000);
Eigen::MatrixXi Indices(3, 10000);
Eigen::MatrixXf C(4, 10000);
Eigen::MatrixXf Floor(3, 10000);
Eigen::MatrixXf Triangle_End(3,24);
Eigen::MatrixXf Triangle_Color(4,24);


Eigen::MatrixXf UV(2, 4);
Eigen::MatrixXf Line(3, 10000);
Eigen::MatrixXf C_Line(4, 10000);


Eigen::Matrix4f projection_view;
Eigen::Matrix4f depth_view;
Eigen::Matrix4f depth_projection_view;
Eigen::Matrix4f roty;


Vector3d camera_position(-1,-1.4,0.8);
Vector3d center(-0.1, 0.025, 0.05);
Vector3d light_position(1, 1, 0);

int vertex_count = 0;
int face_count = 0;

bool gamestart = false;
bool isReady = false;
int succeed = 0;
auto t_start = std::chrono::high_resolution_clock::now();
auto starttime = std::chrono::high_resolution_clock::now();
int angle = 0;
bool preparing = false;
int direction = 1;            // 1-forward
                              // -1-left

int segments = 0;
int end_count = 0;


void render_line() {
	Line.col(0) << -0.075, 0.1, 0.025;
	Line.col(1) << -0.075, 0.1, 0.075;
	Line.col(2) << -0.125, 0.1, 0.075;
	Line.col(3) << -0.125, 0.1, 0.025;

	Line.col(4) << -0.075, 0.05, 0.025;
	Line.col(5) << -0.075, 0.05, 0.075;
	Line.col(6) << -0.125, 0.05, 0.075;
	Line.col(7) << -0.125, 0.05, 0.025;

	Line.col(8) << -0.075, 0.1, 0.075;
	Line.col(9) << -0.075, 0.1, 0.025;
	Line.col(10) << -0.075, 0.05, 0.025;
	Line.col(11) << -0.075, 0.05, 0.075;

	Line.col(12) << -0.125, 0.1, 0.075;
	Line.col(13) << -0.125, 0.1, 0.025;
	Line.col(14) << -0.125, 0.05, 0.025;
	Line.col(15) << -0.125, 0.05, 0.075;

	Line.col(16) << -0.075, 0.1, 0.025;
	Line.col(17) << -0.125, 0.1, 0.025;
	Line.col(18) << -0.125, 0.05, 0.025;
	Line.col(19) << -0.075, 0.05, 0.025;

	Line.col(20) << -0.125, 0.1, 0.075;
	Line.col(21) << -0.075, 0.1, 0.075;
	Line.col(22) << -0.075, 0.05, 0.075;
	Line.col(23) << -0.125, 0.05, 0.075;

	C_Line.col(0) << 1, 0.51, 0.2745, 1;
	C_Line.col(1) << 1, 0.51, 0.2745, 1;
	C_Line.col(2) << 1, 0.51, 0.2745, 1;
	C_Line.col(3) << 1, 0.51, 0.2745, 1;

	C_Line.col(4) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(5) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(6) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(7) << 0.804, 0.408, 0.2235, 1;

	C_Line.col(8) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(9) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(10) << 0.804, 0.408, 0.2235, 1;
	C_Line.col(11) << 0.804, 0.408, 0.2235, 1;

	C_Line.col(12) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(13) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(14) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(15) << 0.93, 0.4745, 0.2588, 1;

	C_Line.col(16) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(17) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(18) << 0.93, 0.4745, 0.2588, 1;
	C_Line.col(19) << 0.93, 0.4745, 0.2588, 1;

	C_Line.col(20) << 1, 0.51, 0.2745, 1;
	C_Line.col(21) << 1, 0.51, 0.2745, 1;
	C_Line.col(22) << 1, 0.51, 0.2745, 1;
	C_Line.col(23) << 1, 0.51, 0.2745, 1;

	VBO_LINE.update(Line);
	VBO_CLINE.update(C_Line);
}

void render_floor() {
//	Floor <<
//		0,   0, -20, -20,
//		0,   0,   0,   0,
//		0,  20,  20,   0;
//	UV <<
//		   1,0,0,1,
//		   1,1,0,0;


//read off file
	FILE *fp;
	const char* fn = "../data/bumpy_cube.off";

	//read off file
	fp = fopen(fn, "r");

	char buffer[1024];
	int node;
	int face;
	int edge;
	float **Ver;
	int **Fa;

	fgets(buffer, 1023, fp);
	float x;
	float y;
	float z;
	int vc = 0;
	int fc = 0;


	if (strstr(buffer, "OFF"))
	{
		fgets(buffer, 1023, fp);
		sscanf(buffer, "%d %d %d", &node, &face, &edge);

		Ver = new float *[node];

		for (int m = 0; m < node; m++)
		{
			fgets(buffer, 1023, fp);
			Ver[m] = new float[3];
			sscanf(buffer, "%f %f %f", &x, &y, &z);
			Ver[m][0] = x;
			Ver[m][1] = y;
			Ver[m][2] = z;

		}


		int o1, o2, o3, o4;
		float color;

		Fa = new int *[face];
		for (int n = 0; n < face; n++)
		{
			fgets(buffer, 1023, fp);
			int temp;
			sscanf(buffer, "%d %d %d %d %d %f", &temp, &o1, &o2, &o3, &o4, &color);
			Fa[n] = new int[4];
			Fa[n][0] = o1;
			Fa[n][1] = o2;
			Fa[n][2] = o3;
			Fa[n][3] = o4;

			if (temp == 4) {
				Floor.col(vc) << Ver[o1][0] + 0.05, 0, -Ver[o1][2];
				Floor.col(vc + 1) << Ver[o2][0] + 0.05, 0, -Ver[o2][2];
				Floor.col(vc + 2) << Ver[o3][0] + 0.05, 0, -Ver[o3][2];
				Floor.col(vc + 3) << Ver[o4][0] + 0.05, 0, -Ver[o4][2];

				fc += 2;
				vc += 4;
			}

		}

	}

	VBO_FLOOR.update(Floor);
//	VBO_UV.update(UV);
}


void render_blocks() {

	//read off file
	FILE *fp;
	const char* fn = "../data/bumpy_cube.off";

	//read off file
	fp = fopen(fn, "r");

	char buffer[1024];
	int node;
	int face;
	int edge;
	float **Ver;
	int **Fa;

	fgets(buffer, 1023, fp);
	float x;
	float y;
	float z;


	if (strstr(buffer, "OFF"))
	{
		fgets(buffer, 1023, fp);
		sscanf(buffer, "%d %d %d", &node, &face, &edge);

		Ver = new float *[node];

		for (int m = 0; m < node; m++)
		{
			fgets(buffer, 1023, fp);
			Ver[m] = new float[3];
			sscanf(buffer, "%f %f %f", &x, &y, &z);
			Ver[m][0] = x;
			Ver[m][1] = y;
			Ver[m][2] = z;

		}


		int o1, o2, o3, o4;
		float color;

		Fa = new int *[face];
		for (int n = 0; n < face; n++)
		{
			fgets(buffer, 1023, fp);
			int temp;
			sscanf(buffer, "%d %d %d %d %d %f", &temp, &o1, &o2, &o3, &o4, &color);
			Fa[n] = new int[4];
			Fa[n][0] = o1;
			Fa[n][1] = o2;
			Fa[n][2] = o3;
			Fa[n][3] = o4;

			if (temp == 4) {
				V.col(vertex_count ) << Ver[o1][0], Ver[o1][1], -Ver[o1][2];
				V.col(vertex_count + 1) << Ver[o2][0], Ver[o2][1], -Ver[o2][2];
				V.col(vertex_count + 2) << Ver[o3][0], Ver[o3][1], -Ver[o3][2];
				V.col(vertex_count + 3) << Ver[o4][0], Ver[o4][1], -Ver[o4][2];

				Indices.col(face_count) << vertex_count , vertex_count + 1, vertex_count  + 2;
				Indices.col(face_count + 1) << vertex_count + 2, vertex_count + 3, vertex_count;


				C.col(vertex_count) << color, color, color, 1;
				C.col(vertex_count + 1) << color, color, color, 1;
				C.col(vertex_count + 2) << color, color, color, 1;
				C.col(vertex_count + 3) << color, color, color, 1;
				face_count += 2;
				vertex_count += 4;
			}
			else {
				Triangle_End.col(vertex_count-3384) << Ver[o1][0], Ver[o1][1], -Ver[o1][2];
				Triangle_End.col(vertex_count - 3384 + 1) << Ver[o2][0], Ver[o2][1], -Ver[o2][2];
				Triangle_End.col(vertex_count - 3384 + 2) << Ver[o3][0], Ver[o3][1], -Ver[o3][2];

				Triangle_Color.col(vertex_count - 3384) << color, color, color, 1;
				Triangle_Color.col(vertex_count - 3384 + 1) << color, color, color, 1;
				Triangle_Color.col(vertex_count - 3384 + 2) << color, color, color, 1;

				face_count++;
				vertex_count += 3;
			}

			

		
		}

//		vertex_count += 4 * face;
//		face_count += 2 * face;
	

	}
	VBO.update(V);
	VBO_C.update(C);
	VBO_END.update(Triangle_End);
	VBO_CEND.update(Triangle_Color);

	EBO.update(Indices);
}

void linePrepare() {

	Eigen::Matrix4f trans;
	trans <<
		1, 0, 0, 0.1,
		0, 1, 0, -0.025,
		0, 0, 1, -0.05,
		0, 0, 0, 1;

	auto now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - t_start).count();
	if (time >= 0.02) {
		for (int i = 0; i < 24; i++) {
			Vector4f x(Line.col(i)[0], Line.col(i)[1], Line.col(i)[2],1);
			x = trans.inverse() * roty * trans * x;
			Line.col(i) << x(0), x(1)-0.0025, x(2);
		}
		angle += 18;
		t_start = now;
		VBO_LINE.update(Line);
	}
	
	if (angle >= 360) {
		preparing = false;
		isReady = true;
	}
}

void success() {
	auto now = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - t_start).count();
	float duration = std::chrono::duration_cast<std::chrono::duration<float>>(now - starttime).count();

	if (duration >= 0.025 * 20) {
		if (time >= 0.025) {
			Line.col(segments * 24 + 1)[2] += 0.01;
			Line.col(segments * 24 + 2)[2] += 0.01;
			Line.col(segments * 24 + 5)[2] += 0.01;
			Line.col(segments * 24 + 6)[2] += 0.01;
			Line.col(segments * 24 + 8)[2] += 0.01;
			Line.col(segments * 24 + 11)[2] += 0.01;
			Line.col(segments * 24 + 12)[2] += 0.01;
			Line.col(segments * 24 + 15)[2] += 0.01;
			Line.col(segments * 24 + 20)[2] += 0.01;
			Line.col(segments * 24 + 21)[2] += 0.01;
			Line.col(segments * 24 + 22)[2] += 0.01;
			Line.col(segments * 24 + 23)[2] += 0.01;

			Line.col(segments * 24 + 1)[0] -= 0.01;
			Line.col(segments * 24 + 2)[0] -= 0.01;
			Line.col(segments * 24 + 5)[0] -= 0.01;
			Line.col(segments * 24 + 6)[0] -= 0.01;
			Line.col(segments * 24 + 8)[0] -= 0.01;
			Line.col(segments * 24 + 11)[0] -= 0.01;
			Line.col(segments * 24 + 12)[0] -= 0.01;
			Line.col(segments * 24 + 15)[0] -= 0.01;
			Line.col(segments * 24 + 20)[0] -= 0.01;
			Line.col(segments * 24 + 21)[0] -= 0.01;
			Line.col(segments * 24 + 22)[0] -= 0.01;
			Line.col(segments * 24 + 23)[0] -= 0.01;
			center(1) += 0.01;
			t_start = now;
		}
	}
	else if (duration >= 0.025 * 20 + 1) {
//		succeed = -1;
	}
	else {
		if (time >= 0.025) {
			Line.col(segments * 24 + 1)[2] += 0.01;
			Line.col(segments * 24 + 2)[2] += 0.01;
			Line.col(segments * 24 + 5)[2] += 0.01;
			Line.col(segments * 24 + 6)[2] += 0.01;
			Line.col(segments * 24 + 8)[2] += 0.01;
			Line.col(segments * 24 + 11)[2] += 0.01;
			Line.col(segments * 24 + 12)[2] += 0.01;
			Line.col(segments * 24 + 15)[2] += 0.01;
			Line.col(segments * 24 + 20)[2] += 0.01;
			Line.col(segments * 24 + 21)[2] += 0.01;
			Line.col(segments * 24 + 22)[2] += 0.01;
			Line.col(segments * 24 + 23)[2] += 0.01;
			camera_position(2) += 0.01;
			center(2) += 0.01;
			t_start = now;

			if (Triangle_End.col(12)[0] < -6.0) {
				Triangle_End.col(12)[0] += 0.01;
				Triangle_End.col(13)[0] += 0.01;
				Triangle_End.col(14)[0] += 0.01;
				Triangle_End.col(15)[0] += 0.01;
				Triangle_End.col(16)[0] += 0.01;
				Triangle_End.col(17)[0] += 0.01;
				Triangle_End.col(18)[0] += 0.01;
				Triangle_End.col(19)[0] += 0.01;
				Triangle_End.col(20)[0] += 0.01;
				Triangle_End.col(21)[0] += 0.01;
				Triangle_End.col(22)[0] += 0.01;
				Triangle_End.col(23)[0] += 0.01;
				VBO_END.update(Triangle_End);
			}

			end_count++;
			if (end_count == 15) {
				segments++;
				Line.col(segments * 24) << Line.col((segments - 1) * 24 + 1)[0], 0.1, 6.2;
				Line.col(segments * 24 + 1) = Line.col(segments * 24);
				Line.col(segments * 24 + 2) << Line.col((segments - 1) * 24 + 2)[0], 0.1, 6.2;
				Line.col(segments * 24 + 3) = Line.col(segments * 24 + 2);
				Line.col(segments * 24 + 4) << Line.col((segments - 1) * 24 + 1)[0], 0, 6.2;
				Line.col(segments * 24 + 5) << Line.col(segments * 24 + 4);
				Line.col(segments * 24 + 6) << Line.col((segments - 1) * 24 + 2)[0], 0, 6.2;
				Line.col(segments * 24 + 7) = Line.col(segments * 24 + 6);
				Line.col(segments * 24 + 8) = Line.col(segments * 24 + 1);
				Line.col(segments * 24 + 9) = Line.col(segments * 24);
				Line.col(segments * 24 + 10) = Line.col(segments * 24 + 4);
				Line.col(segments * 24 + 11) = Line.col(segments * 24 + 5);
				Line.col(segments * 24 + 12) = Line.col(segments * 24 + 2);
				Line.col(segments * 24 + 13) = Line.col(segments * 24 + 3);
				Line.col(segments * 24 + 14) = Line.col(segments * 24 + 7);
				Line.col(segments * 24 + 15) = Line.col(segments * 24 + 6);
				Line.col(segments * 24 + 16) = Line.col(segments * 24);
				Line.col(segments * 24 + 17) = Line.col(segments * 24 + 3);
				Line.col(segments * 24 + 18) = Line.col(segments * 24 + 7);
				Line.col(segments * 24 + 19) = Line.col(segments * 24 + 4);
				Line.col(segments * 24 + 20) = Line.col(segments * 24 + 2);
				Line.col(segments * 24 + 21) = Line.col(segments * 24 + 1);
				Line.col(segments * 24 + 22) = Line.col(segments * 24 + 5);
				Line.col(segments * 24 + 23) = Line.col(segments * 24 + 6);
				C_Line.col(segments * 24 + 0) = C_Line.col((segments - 1) * 24 + 0);
				C_Line.col(segments * 24 + 1) = C_Line.col((segments - 1) * 24 + 1);
				C_Line.col(segments * 24 + 2) = C_Line.col((segments - 1) * 24 + 2);
				C_Line.col(segments * 24 + 3) = C_Line.col((segments - 1) * 24 + 3);
				C_Line.col(segments * 24 + 4) = C_Line.col((segments - 1) * 24 + 4);
				C_Line.col(segments * 24 + 5) = C_Line.col((segments - 1) * 24 + 5);
				C_Line.col(segments * 24 + 6) = C_Line.col((segments - 1) * 24 + 6);
				C_Line.col(segments * 24 + 7) = C_Line.col((segments - 1) * 24 + 7);
				C_Line.col(segments * 24 + 8) = C_Line.col((segments - 1) * 24 + 8);
				C_Line.col(segments * 24 + 9) = C_Line.col((segments - 1) * 24 + 9);
				C_Line.col(segments * 24 + 10) = C_Line.col((segments - 1) * 24 + 10);
				C_Line.col(segments * 24 + 11) = C_Line.col((segments - 1) * 24 + 11);
				C_Line.col(segments * 24 + 12) = C_Line.col((segments - 1) * 24 + 12);
				C_Line.col(segments * 24 + 13) = C_Line.col((segments - 1) * 24 + 13);
				C_Line.col(segments * 24 + 14) = C_Line.col((segments - 1) * 24 + 14);
				C_Line.col(segments * 24 + 15) = C_Line.col((segments - 1) * 24 + 15);
				C_Line.col(segments * 24 + 16) = C_Line.col((segments - 1) * 24 + 16);
				C_Line.col(segments * 24 + 17) = C_Line.col((segments - 1) * 24 + 17);
				C_Line.col(segments * 24 + 18) = C_Line.col((segments - 1) * 24 + 18);
				C_Line.col(segments * 24 + 19) = C_Line.col((segments - 1) * 24 + 19);
				C_Line.col(segments * 24 + 20) = C_Line.col((segments - 1) * 24 + 20);
				C_Line.col(segments * 24 + 21) = C_Line.col((segments - 1) * 24 + 21);
				C_Line.col(segments * 24 + 22) = C_Line.col((segments - 1) * 24 + 22);
				C_Line.col(segments * 24 + 23) = C_Line.col((segments - 1) * 24 + 23);
			}
			
		}
	}
	VBO_LINE.update(Line);
	VBO_CLINE.update(C_Line);
	
}

void failure() {

}

bool check() {
	float x = center(0);
	float z = center(2);
	if (x >= -0.175) {
		if (z <= 0.625) {
			return true;
		}
	}else if (x >= -0.375) {
		if (z <= 0.825 && z >= 0.525) {
			return true;
		}
	}else if (x >= -0.575) {
		if (z <= 1.025 && z >= 0.725) {
			return true;
		}
	}else if (x >= -0.675) {
		if (z <= 1.025 && z >= 0.925) {
			return true;
		}
	}else if (x >= -0.875) {
		if (z <= 1.325 && z >= 0.925) {
			return true;
		}
	}else if (x >= -0.975) {
		if (z <= 1.325 && z >= 1.025) {
			return true;
		}
	}else if (x >= -1.075) {
		if (z <= 1.325 && z >= 1.125) {
			return true;
		}
	}else if (x >= -1.125) {
		if (z <= 1.325 && z >= 1.225) {
			return true;
		}
	}else if (x >= -1.225) {
		if (z <= 1.425 && z >= 1.225) {
			return true;
		}
	}else if (x >= -1.375) {
		if (z <= 1.425 && z >= 1.325) {
			return true;
		}
	}else if (x >= -2.125) {
		if (z <= 1.525 && z >= 1.325) {
			return true;
		}
	}else if (x >= -2.325) {
		if (z <= 1.675 && z >= 1.325) {
			return true;
		}
	}else if (x >= -2.525) {
		if (z <= 1.875 && z >= 1.525) {
			return true;
		}
	}else if (x >= -2.625) {
		if (z <= 2.075 && z >= 1.725) {
			return true;
		}
	}else if (x >= -2.725) {
		if (z <= 2.175 && z >= 1.725) {
			return true;
		}
	}else if (x >= -2.825) {
		if (z <= 2.275 && z >= 2.025) {
			return true;
		}
	}else if (x >= -2.925) {
		if (z <= 2.475 && z >= 2.025) {
			return true;
		}
	}else if (x >= -3.025) {
		if (z <= 2.675 && z >= 2.025) {
			return true;
		}
	}else if (x >= -3.125) {
		if (z <= 3.475 && z >= 2.425) {
			return true;
		}
	}else if (x >= -3.225) {
		if (z <= 3.475 && z >= 2.625) {
			return true;
		}
	}else if (x >= -3.425) {
		if (z <= 3.675 && z >= 3.325) {
			return true;
		}
	}else if (x >= -3.625) {
		if (z <= 3.875 && z >= 3.525) {
			return true;
		}
	}else if (x >= -3.725) {
		if (z <= 3.875 && z >= 3.725) {
			return true;
		}
	}else if (x >= -3.925) {
		if (z <= 4.175 && z >= 3.725) {
			return true;
		}
	}else if (x >= -4.025) {
		if (z <= 4.175 && z >= 3.825) {
			return true;
		}
	}else if (x >= -4.125) {
		if (z <= 4.175 && z >= 3.925) {
			return true;
		}
	}else if (x >= -4.175) {
		if (z <= 4.175 && z >= 4.025) {
			return true;
		}
	}else if (x >= -4.275) {
		if (z <= 4.275 && z >= 4.025) {
			return true;
		}
	}else if (x >= -5.175) {
		if (z <= 4.375 && z >= 4.125) {
			return true;
		}
	}else if (x >= -5.375) {
		if (z <= 4.575 && z >= 4.125) {
			return true;
		}
	}else if (x >= -5.575) {
		if (z <= 4.775 && z >= 4.325) {
			return true;
		}
	}else if (x >= -5.675) {
		if (z <= 4.975 && z >= 4.525) {
			return true;
		}
	}else if (x >= -5.775) {
		if (z <= 5.075 && z >= 4.525) {
			return true;
		}
	}else if (x >= -5.875) {
		if (z <= 5.675 && z >= 4.925) {
			return true;
		}
	}else if (x >= -5.975) {
		if (z <= 5.775 && z >= 4.925) {
			return true;
		}
	}else if (x >= -6.075) {
		if (z <5.975) {
			return true;
		}
		else {
			succeed = 1;
			gamestart = false;
			isReady = false;
			starttime = t_start;
			return false;
		}
	}
	
	succeed = -1;
	gamestart = false;
	isReady = false;
//	PlaySound(TEXT("../data/bang.wav"), NULL, SND_FILENAME);
	return false;
}

void lineRun() {
	
	if (check()) {
		auto now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - t_start).count();

		if (time >= 0.0227) {
			if (direction == 1) {
				Line.col(segments * 24 + 1)[2] += 0.01;
				Line.col(segments * 24 + 2)[2] += 0.01;
				Line.col(segments * 24 + 5)[2] += 0.01;
				Line.col(segments * 24 + 6)[2] += 0.01;
				Line.col(segments * 24 + 8)[2] += 0.01;
				Line.col(segments * 24 + 11)[2] += 0.01;
				Line.col(segments * 24 + 12)[2] += 0.01;
				Line.col(segments * 24 + 15)[2] += 0.01;
				Line.col(segments * 24 + 20)[2] += 0.01;
				Line.col(segments * 24 + 21)[2] += 0.01;
				Line.col(segments * 24 + 22)[2] += 0.01;
				Line.col(segments * 24 + 23)[2] += 0.01;
				camera_position(2) += 0.01;
				center(2) += 0.01;
			}
			else {
				Line.col(segments * 24 + 2)[0] -= 0.01;
				Line.col(segments * 24 + 3)[0] -= 0.01;
				Line.col(segments * 24 + 6)[0] -= 0.01;
				Line.col(segments * 24 + 7)[0] -= 0.01;
				Line.col(segments * 24 + 12)[0] -= 0.01;
				Line.col(segments * 24 + 13)[0] -= 0.01;
				Line.col(segments * 24 + 14)[0] -= 0.01;
				Line.col(segments * 24 + 15)[0] -= 0.01;
				Line.col(segments * 24 + 17)[0] -= 0.01;
				Line.col(segments * 24 + 18)[0] -= 0.01;
				Line.col(segments * 24 + 20)[0] -= 0.01;
				Line.col(segments * 24 + 23)[0] -= 0.01;
				camera_position(0) -= 0.01;
				center(0) -= 0.01;
			}
			t_start = now;
			VBO_LINE.update(Line);
		}
	}
}

void gameReset() {
	succeed = 0;
	segments = 0;
	render_line();
	camera_position = Vector3d(-1, -1.4, 0.8);
	center = Vector3d(-0.1, 0.025, 0.05);
	angle = 0;
	direction = 1;
	end_count = 0;
//	PlaySound(NULL, NULL, SND_FILENAME);
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (succeed == 0) {
			if (!gamestart) {

				if (!isReady) {
					t_start = std::chrono::high_resolution_clock::now();
					preparing = true;
					linePrepare();
				}
				else {
					gamestart = true;
					t_start = std::chrono::high_resolution_clock::now();
					starttime = t_start;
					lineRun();
//					PlaySound(TEXT("../data/background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				}
				
			}
			else {
				segments++;
				if (direction == 1) {
					Line.col(segments * 24 + 0) << Line.col((segments - 1) * 24 + 1)[0], 0.05, Line.col((segments - 1) * 24 + 1)[2] - 0.05;
					Line.col(segments * 24 + 1) << Line.col((segments - 1) * 24 + 1)[0], 0.05, Line.col((segments - 1) * 24 + 1)[2];
					Line.col(segments * 24 + 2) << Line.col((segments - 1) * 24 + 2)[0], 0.05, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 3) << Line.col((segments - 1) * 24 + 2)[0], 0.05, Line.col((segments - 1) * 24 + 2)[2] - 0.05;
					Line.col(segments * 24 + 4) << Line.col((segments - 1) * 24 + 1)[0], 0, Line.col((segments - 1) * 24 + 1)[2] - 0.05;
					Line.col(segments * 24 + 5) << Line.col((segments - 1) * 24 + 1)[0], 0, Line.col((segments - 1) * 24 + 1)[2];
					Line.col(segments * 24 + 6) << Line.col((segments - 1) * 24 + 2)[0], 0, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 7) << Line.col((segments - 1) * 24 + 2)[0], 0, Line.col((segments - 1) * 24 + 2)[2] - 0.05;

				}
				else {

					Line.col(segments * 24 + 0) << Line.col((segments - 1) * 24 + 3)[0] + 0.05, 0.05, Line.col((segments - 1) * 24 + 3)[2];
					Line.col(segments * 24 + 1) << Line.col((segments - 1) * 24 + 2)[0] + 0.05, 0.05, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 2) << Line.col((segments - 1) * 24 + 2)[0], 0.05, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 3) << Line.col((segments - 1) * 24 + 3)[0], 0.05, Line.col((segments - 1) * 24 + 3)[2];
					Line.col(segments * 24 + 4) << Line.col((segments - 1) * 24 + 3)[0] + 0.05, 0, Line.col((segments - 1) * 24 + 3)[2];
					Line.col(segments * 24 + 5) << Line.col((segments - 1) * 24 + 2)[0] + 0.05, 0, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 6) << Line.col((segments - 1) * 24 + 2)[0], 0, Line.col((segments - 1) * 24 + 2)[2];
					Line.col(segments * 24 + 7) << Line.col((segments - 1) * 24 + 3)[0], 0, Line.col((segments - 1) * 24 + 3)[2];

				
				}

				Line.col(segments * 24 + 8) = Line.col(segments * 24 + 1);
				Line.col(segments * 24 + 9) = Line.col(segments * 24 + 0);
				Line.col(segments * 24 + 10) = Line.col(segments * 24 + 4);
				Line.col(segments * 24 + 11) = Line.col(segments * 24 + 5);
				Line.col(segments * 24 + 12) = Line.col(segments * 24 + 2);
				Line.col(segments * 24 + 13) = Line.col(segments * 24 + 3);
				Line.col(segments * 24 + 14) = Line.col(segments * 24 + 7);
				Line.col(segments * 24 + 15) = Line.col(segments * 24 + 6);
				Line.col(segments * 24 + 16) = Line.col(segments * 24 + 0);
				Line.col(segments * 24 + 17) = Line.col(segments * 24 + 3);
				Line.col(segments * 24 + 18) = Line.col(segments * 24 + 7);
				Line.col(segments * 24 + 19) = Line.col(segments * 24 + 4);
				Line.col(segments * 24 + 20) = Line.col(segments * 24 + 2);
				Line.col(segments * 24 + 21) = Line.col(segments * 24 + 1);
				Line.col(segments * 24 + 22) = Line.col(segments * 24 + 5);
				Line.col(segments * 24 + 23) = Line.col(segments * 24 + 6);

				C_Line.col(segments * 24 + 0) = C_Line.col((segments - 1) * 24 + 0);
				C_Line.col(segments * 24 + 1) = C_Line.col((segments - 1) * 24 + 1);
				C_Line.col(segments * 24 + 2) = C_Line.col((segments - 1) * 24 + 2);
				C_Line.col(segments * 24 + 3) = C_Line.col((segments - 1) * 24 + 3);
				C_Line.col(segments * 24 + 4) = C_Line.col((segments - 1) * 24 + 4);
				C_Line.col(segments * 24 + 5) = C_Line.col((segments - 1) * 24 + 5);
				C_Line.col(segments * 24 + 6) = C_Line.col((segments - 1) * 24 + 6);
				C_Line.col(segments * 24 + 7) = C_Line.col((segments - 1) * 24 + 7);
				C_Line.col(segments * 24 + 8) = C_Line.col((segments - 1) * 24 + 8);
				C_Line.col(segments * 24 + 9) = C_Line.col((segments - 1) * 24 + 9);
				C_Line.col(segments * 24 + 10) = C_Line.col((segments - 1) * 24 + 10);
				C_Line.col(segments * 24 + 11) = C_Line.col((segments - 1) * 24 + 11);
				C_Line.col(segments * 24 + 12) = C_Line.col((segments - 1) * 24 + 12);
				C_Line.col(segments * 24 + 13) = C_Line.col((segments - 1) * 24 + 13);
				C_Line.col(segments * 24 + 14) = C_Line.col((segments - 1) * 24 + 14);
				C_Line.col(segments * 24 + 15) = C_Line.col((segments - 1) * 24 + 15);
				C_Line.col(segments * 24 + 16) = C_Line.col((segments - 1) * 24 + 16);
				C_Line.col(segments * 24 + 17) = C_Line.col((segments - 1) * 24 + 17);
				C_Line.col(segments * 24 + 18) = C_Line.col((segments - 1) * 24 + 18);
				C_Line.col(segments * 24 + 19) = C_Line.col((segments - 1) * 24 + 19);
				C_Line.col(segments * 24 + 20) = C_Line.col((segments - 1) * 24 + 20);
				C_Line.col(segments * 24 + 21) = C_Line.col((segments - 1) * 24 + 21);
				C_Line.col(segments * 24 + 22) = C_Line.col((segments - 1) * 24 + 22);
				C_Line.col(segments * 24 + 23) = C_Line.col((segments - 1) * 24 + 23);
				VBO_LINE.update(Line);
				VBO_CLINE.update(C_Line);
				direction = -direction;
				
			}
		}
		else{
			gameReset();
		}
		
	}

}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}


int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(500, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));



    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
	VBO.init();
	VBO.update(V);

	VBO_C.init();
	VBO_C.update(C);

	VBO_FLOOR.init();
	VBO_FLOOR.update(Floor);

	VBO_UV.init();
	VBO_UV.update(UV);

	VBO_LINE.init();
	VBO_LINE.update(Line);

	VBO_CLINE.init();
	VBO_CLINE.update(C_Line);

	VBO_END.init();
	VBO_END.update(Triangle_End);

	VBO_CEND.init();
	VBO_CEND.update(Triangle_Color);

	EBO.init();
	EBO.update(Indices);
	
	Program program_shadow;
	const GLchar* vertex_shader_shadow =
		"#version 150 core\n"
		"in vec3 vertexPosition_modelspace;"
		"uniform mat4 DepthView;"
//		"out vec4 cgl_texCoordProj;"
		"void main()"
		"{"
//		"    cgl_texCoordProj =  DepthView * vec4(vertexPosition_modelspace, 1.0);"
		"    gl_Position =  DepthView * vec4(vertexPosition_modelspace, 1.0);"

		"}";
	const GLchar* fragment_shader_shadow =
		"#version 150 core\n"
//		"out float fragmentdepth;"
        "out vec4 color;"
		"void main()"
		"{"
//		"    fragmentdepth = gl_FragCoord.z;"
        "    color = vec4(0,0,0,1);"
		"}";

	Program program_floor;
	const GLchar* vertex_shader_floor =
		"#version 150 core\n"
		"in vec3 vertexPosition_modelspace;"
		"out vec4 ShadowCoord;"
		"uniform mat4 MVP;"
		"uniform mat4 DepthBiasMVP;"
		"void main()"
		"{"
		"    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
		"    ShadowCoord = DepthBiasMVP * vec4(vertexPosition_modelspace,1);"

		"}";
	const GLchar* fragment_shader_floor =
		"#version 150 core\n"
		"in vec4 ShadowCoord;"
		"out vec4 color;"
		"uniform sampler2DShadow shadowMap;"
		"void main()"
		"{"
		"   color = textureProj(shadowMap,ShadowCoord)*vec4(0.94, 0.63, 0.36,1);"
		"}";
					
	Program program;
	const GLchar* vertex_shader =
		"#version 150 core\n"
		"in vec3 position;"
		"in vec4 color;"
		"out vec4 vertex_color;"
		"uniform mat4 view;"
		"void main()"
		"{"
		"    gl_Position =  view * vec4(position, 1.0);"
		"    vertex_color = color;"
		"}";
	const GLchar* fragment_shader =
		"#version 150 core\n"
		"out vec4 outColor;"
		"in vec4 vertex_color;"
		"void main()"
		"{"
		"    outColor = vertex_color;"
		"}";

	Program program_line;
	const GLchar* vertex_shader_line =
		"#version 150 core\n"
		"in vec3 position;"
		"uniform mat4 view;"
		"void main()"
		"{"
		"    gl_Position =  view * vec4(-0.1,0.025,0.05, 1.0);"
		"}";
	const GLchar* fragment_shader_line =
		"#version 150 core\n"
		"out vec4 outColor;"
		"void main()"
		"{"
		"    outColor = vec4(1,0,0,1);"
		"}";

		
    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
	program_line.init(vertex_shader_line, fragment_shader_line, "outColor");
	program_shadow.init(vertex_shader_shadow, fragment_shader_shadow, "color");
	program_floor.init(vertex_shader_floor, fragment_shader_floor, "color");

	roty <<
		cos(-18 * 3.1415926 / 180), 0, sin(-18 * 3.1415926 / 180), 0,
		0, 1, 0, 0,
		-sin(-18 * 3.1415926 / 180), 0, cos(-18 * 3.1415926 / 180), 0,
		0, 0, 0, 1;

	render_blocks();
	render_floor();
	render_line();
//	mciSendString(TEXT("open background.wav alias mysong"), NULL, 0, NULL);

	float n = 5;
	float f = -5;
	float r = 0.7;
	float l = -0.3;
	float t = 0.9;
	float b = -0.9;
	projection_view <<
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, 2 / (n - f), -(n + f) / (n - f),
		0, 0, 0, 1;

	Vector3d u_shadow;
	Vector3d v_shadow;
	Vector3d w_shadow;

	w_shadow = light_position - Vector3d(0,0,0);
	w_shadow.normalize();

	u_shadow = Vector3d(0, 1, 0).cross(w_shadow);
	u_shadow.normalize();

	v_shadow = w_shadow.cross(u_shadow); // up = side x look
	v_shadow.normalize();

	depth_view <<
		u_shadow(0), u_shadow(1), u_shadow(2), -light_position(0)*u_shadow(0) - light_position(1)*u_shadow(1) - light_position(2)*u_shadow(2),
		v_shadow(0), v_shadow(1), v_shadow(2), -light_position(0)*v_shadow(0) - light_position(1)*v_shadow(1) - light_position(2)*v_shadow(2),
		w_shadow(0), w_shadow(1), w_shadow(2), -light_position(0)*w_shadow(0) - light_position(1)*w_shadow(1) - light_position(2)*w_shadow(2),
		0, 0, 0, 1;

	Eigen::Matrix4f bias;
	bias <<
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0;

	 n = 5;
	 f = -5;
	 r = 10;
	 l = -10;
	 t = 3;
	 b = -3;
	depth_projection_view <<
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, 2 / (n - f), -(n + f) / (n - f),
		0, 0, 0, 1;

	Eigen::Matrix4f depth_bias_view;
	depth_bias_view = bias * depth_projection_view*depth_view;

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

//	GLuint img;
//	glGenTextures(1, &img);
//	glBindTexture(GL_TEXTURE_2D, img);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, img, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

						
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

		if (preparing) {
			linePrepare();
		}
		if (gamestart) {
			lineRun();
		}
		if (succeed == 1) {
			success();
		}
		else if (succeed == -1) {
			failure();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, 1024, 1024); 

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 

        glClearColor(0.99f, 0.68f, 0.41f, 0.5f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		Vector3d u;
		Vector3d v;
		Vector3d w;

		w = camera_position - center;
		w.normalize();

		u = Vector3d(0, 1, 0).cross(w);
		u.normalize();

		v = w.cross(u); // up = side x look
		v.normalize();

		camera_view <<
			u(0), u(1), u(2), -camera_position(0)*u(0)- camera_position(1)*u(1)- camera_position(2)*u(2),
			v(0), v(1), v(2), -camera_position(0)*v(0) - camera_position(1)*v(1) - camera_position(2)*v(2),
			w(0), w(1), w(2), -camera_position(0)*w(0) - camera_position(1)*w(1) - camera_position(2)*w(2),
			0, 0, 0, 1;

		Eigen::Matrix4f view;
		view = projection_view * camera_view;


		program_shadow.bind();
		program_shadow.bindVertexAttribArray("vertexPosition_modelspace", VBO);
		glUniformMatrix4fv(program_shadow.uniform("DepthView"), 1, GL_FALSE, depth_bias_view.data());
		glDrawElements(GL_TRIANGLES, (face_count - 8) * 3, GL_UNSIGNED_INT, 0);
		


		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

							 // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program_floor.bind();
		program_floor.bindVertexAttribArray("vertexPosition_modelspace", VBO_FLOOR);
		glUniformMatrix4fv(program_floor.uniform("MVP"), 1, GL_FALSE, view.data());
		glUniformMatrix4fv(program_floor.uniform("DepthBiasMVP"), 1, GL_FALSE, depth_bias_view.data());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(program_floor.uniform("shadowMap"), 0);
		glDrawElements(GL_TRIANGLES, (face_count - 8) * 3, GL_UNSIGNED_INT, 0);
		
		
		
		
		//load piano blocks background
		program.bind();
		program.bindVertexAttribArray("position", VBO);
		program.bindVertexAttribArray("color", VBO_C);
		glUniformMatrix4fv(program.uniform("view"), 1, GL_FALSE, view.data());
		glDrawElements(GL_TRIANGLES, (face_count-8) * 3, GL_UNSIGNED_INT, 0);

		program.bindVertexAttribArray("position", VBO_END);
		program.bindVertexAttribArray("color", VBO_CEND);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		program.bindVertexAttribArray("position", VBO_LINE);
		program.bindVertexAttribArray("color", VBO_CLINE);
		glDrawElements(GL_TRIANGLES, (segments+1) * 36, GL_UNSIGNED_INT, 0);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    program.free();
	program_line.free();
	program_floor.free();
	program_shadow.free();


    VAO.free();
    VBO.free();
	VBO_C.free();
	VBO_CLINE.free();
	VBO_FLOOR.free();
	VBO_UV.free();
	VBO_END.free();
	VBO_CEND.free();
	EBO.free();



    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
