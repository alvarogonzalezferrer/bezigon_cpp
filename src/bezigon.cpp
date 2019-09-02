// ------------------------------------
// bezigon.cpp
// ------------------------------------
// Class for handle bezigons
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
#include "bezigon.h"

int Bezigon::quality = 32 ; // default quality rendering


Bezigon::Bezigon()
{
	// init default state.

	this->nodes.clear();
	
	this->dirty = FALSE;
	
	color_fill = makecol(255,255,255); // default: white color :P
	
	// debug options
	show_tangents = FALSE;
	show_control_points = FALSE;
	show_debug_points = FALSE;
	
	// polygon and spline buffer
	polybuf = NULL;
	polybuf_size = 0;
	bezbufx = NULL;
   bezbufy = NULL; 
}

Bezigon::~Bezigon()
{
	this->nuke_nodes(); // clean the memory mess
}


// ------------------------------------
// Nukes the nodes list
// ------------------------------------
void Bezigon::nuke_nodes()
{
	for(int a = 0; a < this->nodes.size(); a++)
		delete  nodes[a]; // free memory

	this->nodes.clear();

	this->dirty = TRUE;

	this->nuke_poly_buffer();
}

// ------------------------------------
// Nukes the nodes list
// ------------------------------------
void Bezigon::nuke_poly_buffer()
{
	if (polybuf != NULL)
	{
		delete [] polybuf;
		polybuf = NULL;
		polybuf_size = 0;
	}

	if (bezbufx !=NULL)
	{
		delete [] bezbufx;
		bezbufx = NULL;
	}
	
	if (bezbufy !=NULL)
	{
		delete [] bezbufy;
		bezbufy = NULL;
	}

	this->dirty = TRUE;
}

// ------------------------------------
// Quality set
// ------------------------------------
void Bezigon::set_quality(int q)
{
	if (q<4) q = 4;
	if (q>256) q = 256;
	quality = q;
	this->dirty = TRUE;
	
	// when quality changes, we must free the polybuf buffer
	this->nuke_poly_buffer();
}

// ------------------------------------
// This creates and adds a new node to the bezigon
// ------------------------------------
void Bezigon::cadd_node(int x, int y, float curviness)
{
	SplineNode *n;

	n = new SplineNode(x,y,0,curviness);

	this->add_node(n);

}

// ------------------------------------
// This adds a new node to the bezigon
// ------------------------------------
void Bezigon::add_node(SplineNode *n)
{
	this->nodes.push_back(n);
	
	this->dirty = TRUE;
}

// ------------------------------------
// This draws the bezigon
// ------------------------------------
void Bezigon::draw(BITMAP *bmp)
{

	// if is 'dirty' we must first calc the polygon	
	if (this->dirty)
	{
		this->calculate_polygon();
	}
	
	// Debug drawing (I compare the 2 in one to avoid doing many times the for)
	if (show_control_points || show_tangents)
	{	
		for (int a =0; a < nodes.size(); a++)
		{
			if (show_control_points)
   			circlefill(bmp, nodes[a]->x, nodes[a]->y, 2, makecol(255,0,255));
			
			if (show_tangents)
			{
				line(screen, nodes[a]->get_x() - (int)(cos(nodes[a]->get_tangent()) * 24.0),
							 nodes[a]->get_y() - (int)(sin(nodes[a]->get_tangent()) * 24.0),
							 nodes[a]->get_x() + (int)(cos(nodes[a]->get_tangent()) * 24.0),
							 nodes[a]->get_y() + (int)(sin(nodes[a]->get_tangent()) * 24.0), makecol(0,255,255) );
			}
		}
	}
		
	if (show_debug_points)
	{
		for (int a=0; a<polybuf_size; a+=2)
		{
		 circlefill(bmp, polybuf[a], polybuf[a+1], 1, makecol(255,255,0));
		}
	}
	
	// Draw the polygon
	polygon(bmp, polybuf_size/2, polybuf, color_fill );
}

// ------------------------------------
// This is a debug function, dumps the nodes info
// ------------------------------------
void Bezigon::dump_print_nodes()
{
	cout << "nodes.size(): " << nodes.size() << endl;
	cout << "dirty flag: " << this->dirty << endl;
	cout << "polybuf_size: " << this->polybuf_size << endl;
	cout << "quality: " << this->quality << endl;
	for (int a =0; a < nodes.size(); a++)
	{
		cout << "node[" << a << "] = xy(" << nodes[a]->get_x() << "," << nodes[a]->get_y() <<") ; ";
		cout << "curv(" <<   nodes[a]->get_curviness()  << ") ; tan(" << nodes[a]->get_tangent()  << ")" << endl;
	}
}

// ------------------------------------
// This calculates the polygon buffer
// Is done automatically before drawing
// this also sets dirty flag to FALSE
// ------------------------------------
void Bezigon::calculate_polygon()
{
	int spl[8] ; // coords of the spline control points
	int i = 0, i2 = 0, i3 = 0; // aux vars
	float distn = 0;
	

	polybuf_size = 0; // reset the polygon buffer, no matter what
	
	// if the polygon is dirty, we must recreate the memory buffers
	if (this->dirty)
	{
		this->nuke_poly_buffer(); // bye to old buffers

	// malloc the buffers 
		polybuf = new int[ nodes.size() * 4 * this->quality ]; // allocates some more memory that needed (DEBUG!)
		if (polybuf == NULL) return ;
		
		bezbufx = new int [ this->quality*2 ];
		if (bezbufx == NULL) // DEBUG: should use exceptions to catch the error!!!!
		{	
			delete [] polybuf;
			return; // ERROR 
		}
		
		bezbufy =  new int[ this->quality*2 ];
		if (bezbufy == NULL) 
		{
			delete [] polybuf; 
			delete [] bezbufx; 
			return; // ERROR
		}
	
	}
 
	// calculate all and make the polygon in buffer
	
	this->dirty = TRUE; // if something happens, we are still dirty :P
	
	if (nodes.size () < 2) return ; // we need 2 points at least!


	 	for (i=0; i < nodes.size(); i++ )
	{
		// take two control points
		spl[0] = nodes[i]->get_x();
		spl[1] = nodes[i]->get_y();
	
		i2 = i + 1;
		if (i2 >= nodes.size() ) i2 = 0; // wrap around the control points,so the final point connect to first point
	
		spl[6] = nodes[i2]->get_x();
		spl[7] =	nodes[i2]->get_y();

		// calc tangent of each control point
		// the index wraps around, for that I use two aux vars
	
		// begin node of spline
		i2 = i - 1;
		if (i2 < 0) i2 = nodes.size()-1;
	
		i3 = i + 1; 
		if (i3 >= nodes.size()) i3 = 0;
		
		nodes[i]->set_tangent( atan2( (float)(nodes[i3]->get_y() - nodes[i2]->get_y()) , (float)(nodes[i3]->get_x() - nodes[i2]->get_x()) ) );
	
		// end node of spline
		i2 = i + 1;
		if (i2 >= nodes.size() ) i2 = 0;
	
		i3 = i + 2; 
		if (i3 >= nodes.size()) i3 = 0;
	
		nodes[i2]->set_tangent( atan2( (float)(nodes[i3]->get_y() - nodes[i]->get_y()) , (float)(nodes[i3]->get_x() - nodes[i]->get_x()) ) );
		
		//
		// NO modify i or i2 here, I need them intact below
		// 

		// make the two middle control points based on curviness,tangents and distance between control points
		//
		
		distn =  SplineNode::get_node_dist(nodes[i], nodes[i2]) * nodes[i]->get_curviness() ;

		spl[2] = nodes[i]->get_x() + (int)(cos(nodes[i]->get_tangent()) * distn);
		spl[3] = nodes[i]->get_y() + (int)(sin(nodes[i]->get_tangent()) * distn);

		spl[4] = nodes[i2]->get_x() - (int)(cos(nodes[i2]->get_tangent()) * distn);
		spl[5] = nodes[i2]->get_y() - (int)(sin(nodes[i2]->get_tangent()) * distn);

		// calc the spline
		
		calc_spline(spl, this->quality, bezbufx, bezbufy); // calc the spline

		// copy the points to the poly buf
		for (i2 = 0; i2 < this->quality; i2++)
		{
				polybuf[polybuf_size] = bezbufx[i2];
                polybuf[polybuf_size+1] = bezbufy[i2];
                polybuf_size += 2;
		}

	}
	
	this->dirty = FALSE; // all nice, we can draw
}



