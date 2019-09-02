// ------------------------------------
// node.h
// ------------------------------------
// Class for handle spline nodes
// By Kronoman
// In loving memory of my father
// Copyright (c) 2003, Kronoman
// Made in Argentina
// ------------------------------------
// NOTICE:
// some of the implementation was taken
// from exspline.c, from Allegro Libray
// ------------------------------------
#ifndef NODE_H
#define NODE_H

#include <allegro.h>

class SplineNode
{
	public:
		SplineNode();
		SplineNode(int xp=0, int yp=0,  float tangentp=0, float curvinessp=0);
		~SplineNode();

		void set_x(int xp);
		void set_y(int yp);
		
		int get_x();
		int get_y();

		float get_tangent();
		void set_tangent(float tt);

		float get_curviness();
		void set_curviness(float c);


		// some node utilities:
		static float get_node_dist(SplineNode *n1, SplineNode *n2);       // calc the distance between two nodes
		
		// in this class, the data is public, so you can access it fast
		// you can, however, use the metods above mentioned.

		int x, y;
		float tangent;
		float curviness; // amount of curviness of this node
};

#endif
