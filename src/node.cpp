// ------------------------------------
// node.cpp
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
#include <allegro.h>
#include <math.h>
#include "node.h"

float SplineNode::get_node_dist(SplineNode *n1, SplineNode *n2)
{
	float dx = n1->get_x() - n2->get_x();
	float dy = n1->get_y() - n2->get_y();
	return sqrt( dx*dx + dy*dy ); // viejo Pitagoras
}


SplineNode::SplineNode()
{
	// default values
   x = y = 0;
	tangent = 0.0;
   curviness = 0.25;
}

SplineNode::SplineNode(int xp, int yp, float tangentp, float curvinessp)
{
	x = xp;
	y = yp;
	tangent = tangentp;
	curviness = curvinessp;
}


SplineNode::~SplineNode()
{
	
}
	
void SplineNode::set_x(int xp)
{
	 x = xp;
}

void SplineNode::set_y(int yp)
{
	 y = yp;
}

int SplineNode::get_x()
{
	return x;
}

int SplineNode::get_y()
{
	return y;
}

void SplineNode::set_tangent(float tt)
{
	tangent = tt;
}

float SplineNode::get_tangent()
{
	return tangent;
}


float SplineNode::get_curviness()
{
	return curviness;
}

void SplineNode::set_curviness(float c)
{
	curviness = c;
}
