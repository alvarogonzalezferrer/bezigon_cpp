// ------------------------------------
// bezigon.h
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

#ifndef BEZIGON_H
#define BEZIGON_H


#include <vector> // STL container for the bezigon nodes
#include <iostream>
using namespace std; // needed for some compilers to use the STL

#include "node.h" // the node class

#include <allegro.h> // our loved allegro

class Bezigon 
{
	public:
		Bezigon();
		~Bezigon();

		void draw(BITMAP *bmp ); // draws the bezigon on bmp

		void cadd_node(int x, int y, float curviness); // creates and adds a node of the contour of the bezigon
		void add_node(SplineNode *node); // adds a already created node

		void set_color_fill(int c=0) { color_fill = c; }; // sets the fill color
		int get_color_fill() const { return color_fill; }; // gets the fill color
		
		void set_quality(int q=48); // sets the quality of rendering
		int get_quality() const { return quality; }; // returns the rendering quality
		
		void nuke_nodes(); // this deletes the nodes and free the vector, and also nukes the poly buffer
		void nuke_poly_buffer(); // this deletes the poly buffer (free the RAM)

		void set_dirty_flag(bool d) { this->dirty = d; }; // toggle dirty flag
		bool get_dirty_flag() const { return this->dirty; } ; // get dirty flag


		void calculate_polygon(); // this does the trick of spline to polygon conversion
		
		
		// some interesting functions to control the whole bezigon:
		void set_bezigon_curviness(float curv)	 { for (int a = 0; a < nodes.size(); a++) nodes[a]->set_curviness(curv); }; // sets the curviness for *all* control points
		void translate_bezigon(int x, int y); // displaces the bezigon by x,y 
		void rotate_bezigon(float angle); // rotates the bezigon, angle in radians
		void scale_bezigon(float ratio); // scales the bezigon by ratio
		
		// debug stuff
		
		void set_show_tangents(bool show_t) { show_tangents = show_t; }; // show tangents?
		void set_show_control_points(bool show_c) { show_control_points = show_c;} ; // show control points?
		void set_show_debug_points(bool show_p) { show_debug_points = show_p; }; // show spline and other misc points?
		
		void dump_print_nodes(); // dump the nodes to text screen - useful for debug purpose

		
		
		// DEBUG: add functions to read/save to stream
		
   // --------------------------------------------------------------------
	private:
		// The splines are stored as a set of nodes (where the spline pass) 
		// with a "curviness" that controls the guide points 
		// (the points that made the shape of the spline)
		// the guide points are made on-fly based on curviness
		
		vector <SplineNode *> nodes;

		int *polybuf; // this holds the buffer with polygon nodes (it has to be like this, to use with Allegro's C functions)
		int polybuf_size; // how big is polybuf? ( divide by 2 for pass polybuf to polygon() )
		
		int *bezbufx;  // buffer for calc spline coords 
		int *bezbufy; 

		bool dirty; // is 'dirty'? this means if we have to re calculate te polybuf (i.e when a new node is add)

		// Rendering options
		
		int color_fill; // color of the bezigon

			
		// Debug options
		
		// this are more for debug	, and affect the drawing only
		bool show_tangents;
		bool show_control_points;
		bool show_debug_points;

		// Vars for all the Bezigon objects
		
		static int quality; // quality of rendering
};
#endif

