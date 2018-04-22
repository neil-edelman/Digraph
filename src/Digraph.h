/** 2016 Neil Edelman, distributed under the terms of the MIT License;
 see readme.txt, or \url{ https://opensource.org/licenses/MIT }.

 {<G>Digraph} is an abstract directed graph represented by adjacency lists
 that is backed by {<G>Vertex} and {<G>Edge}. It does very little except expose
 the data types. In particular, there only option by default to know if a graph
 contains an element is an exhaustive search. The preprocessor macros are all
 undefined at the end of the file for convenience.

 @param DIGRAPH_NAME
 This literally becomes {<G>}. As it's used in function names, this should
 comply with naming rules and be unique; required.

 @param DIGRAPH_VDATA, DIGRAPH_EDATA
 The optional type(s) associated with {<V>} and {<E>} to store on each vertex
 and edge. Have to be valid types, accessible to the compiler at the time of
 inclusion. For example, if one were doing DFS, you would include some sort of
 structure that had visited in {DIGRAPH_VDATA}, or colour for graph colouring,
 or distance for Dijkstra's or A*. One can set a {start} vertex with
 \see{<G>DigraphStart}, but depending on the algorithm, one may need {is_sink}.

 @param DIGRAPH_VDATA_TO_STRING, DIGRAPH_EDATA_TO_STRING
 Optional print function(s) implementing {<G>VDataToString} and
 {<G>EDataToString}.

 @param DIGRAPH_TEST, DIGRAPH_VDATA_TEST, DIGRAPH_EDATA_TEST
 Unit testing framework using {<G>DigraphTest}, included in a separate header,
 {../test/DigraphTest.h}. If {DIGRAPH_TEST},
 {DIGRAPH_VDATA <-> DIGRAPH_VDATA_TEST} and
 {DIGRAPH_EDATA <-> DIGRAPH_EDATA_TEST}, which must be defined equal to a
 (random) filler function, satisfying {<G>VDataAction} and {<E>EDataAction}.
 If {NDEBUG} is not defined, turns on {assert} private function integrity
 testing.

 @title		Digraph.h
 @std		C89
 @author	Neil
 @version	2018-04 This is cool. */



#include <stdio.h>  /* FILE for {<G>DigraphOut} */
#include <assert.h>	/* assert */
#include <stdio.h>	/* snprintf */
#include <errno.h>	/* errno */



/* Check defines. */
#ifndef DIGRAPH_NAME /* <-- error */
#error Generic DIGRAPH_NAME undefined.
#endif /* error --> */
#if defined(DIGRAPH_VDATA_TO_STRING) && !defined(DIGRAPH_VDATA) \
	|| defined(DIGRAPH_EDATA_TO_STRING) && !defined(DIGRAPH_EDATA)
	/* <-- error */
#error DATA_TO_STRING without DATA.
#endif /* error --> */
#ifdef DIGRAPH_TEST /* <-- test */
#if defined(DIGRAPH_VDATA) && !defined(DIGRAPH_VDATA_TEST) || \
	defined(DIGRAPH_EDATA) && !defined(DIGRAPH_EDATA_TEST) /* <-- error */
#error DIGRAPH_TEST requires DIGRAPH_VDATA_TEST and/or DIGRAPH_EDATA_TEST.
#endif /* error --> */
#else /* test--><-- !test */
#if defined(DIGRAPH_VDATA_TEST) || defined(DIGRAPH_EDATA_TEST) /* <-- error */
#error DIGRAPH_VDATA_TEST and DIGRAPH_EDATA_TEST require DIGRAPH_TEST.
#endif /* error --> */
#ifndef NDEBUG /* <-- ndebug */
#define DIGRAPH_NDEBUG
#define NDEBUG
#endif /* ndebug --> */
#endif /* !test --> */



/* Generics using the preprocessor;
 \url{ http://stackoverflow.com/questions/16522341/pseudo-generics-in-c }. */
#ifdef CAT
#undef CAT
#endif
#ifdef CAT_
#undef CAT_
#endif
#ifdef PCAT
#undef PCAT
#endif
#ifdef PCAT_
#undef PCAT_
#endif
#ifdef V
#undef V
#endif
#ifdef E
#undef E
#endif
#ifdef G_
#undef G_
#endif
#ifdef PG_
#undef PG_
#endif
#ifdef QUOTE
#undef QUOTE
#endif
#ifdef QUOTE_
#undef QUOTE_
#endif
#define CAT_(x, y) x ## y
#define CAT(x, y) CAT_(x, y)
#define PCAT_(x, y) x ## _ ## y
#define PCAT(x, y) PCAT_(x, y)
#define G_(thing) CAT(DIGRAPH_NAME, thing)
#define PG_(thing) PCAT(digraph, PCAT(DIGRAPH_NAME, thing))
#define QUOTE_(name) #name
#define QUOTE(name) QUOTE_(name)



/* Troubles with this line? check to ensure that {DIGRAPH_VDATA} is a valid
 type, whose definition is placed above {#include "Digraph.h"}. */
#ifdef DIGRAPH_VDATA /* <-- vdata */
typedef DIGRAPH_VDATA PG_(VData);
#define V PG_(VData)
#endif /* vdata --> */

/* Troubles with this line? check to ensure that {DIGRAPH_EDATA} is a valid
 type, whose definition is placed above {#include "Digraph.h"}. */
#ifdef DIGRAPH_EDATA /* <-- edata */
typedef DIGRAPH_EDATA PG_(EData);
#define E PG_(EData)
#endif /* egde --> */



#ifdef DIGRAPH_VDATA_TO_STRING /* <-- v2string */
/** Responsible for turning {<V>} (the first argument) into a 12 {char}
 null-terminated output string (the second.) */
typedef void (*PG_(VDataToString))(const V *, char (*const)[12]);
/* Check that {DIGRAPH_VDATA_TO_STRING} is a function implementing
 {<PVE>VDataToString}. */
static const PG_(VDataToString) PG_(vdata_to_string) =(DIGRAPH_VDATA_TO_STRING);
#endif /* v2string --> */

#ifdef DIGRAPH_EDATA_TO_STRING /* <-- e2string */
/** Responsible for turning {<E>} (the first argument) into a 12 {char}
 null-terminated output string (the second.) */
typedef void (*PG_(EDataToString))(const E *, char (*const)[12]);
/* Check that {DIGRAPH_EDATA_TO_STRING} is a function implementing
 {<PVE>EDataToString}. */
static const PG_(EDataToString) PG_(edata_to_string) =(DIGRAPH_EDATA_TO_STRING);
#endif /* e2string --> */

#ifdef DIGRAPH_TEST /* <-- test */
#ifdef DIGRAPH_VDATA /* <-- vdata */
/** Performs an action on a vertex-associated {<V>}. */
typedef void (*PG_(VDataAction))(V *const);
#endif /* vdata --> */
#ifdef DIGRAPH_EDATA /* <-- edata */
/** Performs an action on an edge-associated {<E>}. */
typedef void (*PG_(EDataAction))(E *const);
#endif /* edata --> */
#endif /* test -->*/



/** Edge. */
struct G_(Edge);
struct G_(Vertex);
struct G_(Edge) {
#ifdef DIGRAPH_EDATA /* <-- edata */
	E data;
#endif /* edata --> */
	struct G_(Vertex) *to;
};

/** @implements <<G>Edge>ToString */
static void PG_(edge_to_string)(const struct G_(Edge) *const e,
	char (*const a)[12]) {
#ifdef DIGRAPH_EDATA /* <-- edata */
	PG_(edata_to_string)(&e->data, a);
#else /* edata --><-- !edata */
	strcpy(*a, "e");
	(void)e;
#endif /* !edata --> */
}

/* This relies on {List.h} which must be in the same directory. */
#define LIST_NAME G_(Edge)
#define LIST_TYPE struct G_(Edge)
#define LIST_SUBTYPE
#define LIST_TO_STRING &PG_(edge_to_string)
#include "List.h" /* Defines {<G>EdgeList} and {<G>EdgeListNode}. */

/** Vertex. */
struct G_(Vertex);
struct G_(Vertex) {
#ifdef DIGRAPH_VDATA /* <-- vdata */
	V data;
#endif /* vdata --> */
	struct G_(EdgeList) out;
};

/** @fixme Is this used???
 @implements <<G>Vertex>ToString */
static void PG_(vertex_to_string)(const struct G_(Vertex) *const v,
	char (*const a)[12]) {
#ifdef DIGRAPH_VDATA /* <-- vdata */
	PG_(vdata_to_string)(&v->data, a);
#else /* vdata --><-- !vdata */
	strcpy(*a, "v");
	(void)v;
#endif /* !vdata --> */
}

/* This relies on {List.h} which must be in the same directory. */
#define LIST_NAME G_(Vertex)
#define LIST_TYPE struct G_(Vertex)
#define LIST_TO_STRING &PG_(vertex_to_string)
#define LIST_SUBTYPE
#include "List.h" /* Defines {<G>VertexList} and {<G>VertexListNode}. */

/** The directed graph. To instantiate, see \see{<V>Digraph}. */
struct G_(Digraph);
struct G_(Digraph) {
	struct G_(VertexList) vertices;
	struct G_(Vertex) *start;
};



/** Called in \see{<G>Digraph_}. Does nothing for vertex data.
 @implements <G>VertexAction */
static void PG_(v_clear)(struct G_(Vertex) *const v) {
	assert(v);
	G_(EdgeListClear)(&v->out);
}

/** Initialises the graph to empty. */
static void PG_(clear)(struct G_(Digraph) *const g) {
	G_(VertexListClear)(&g->vertices);
	g->start = 0;
}

/** Destructor for {g}.
 @param g: If null or empty, does nothing.
 @order O({vertices})
 @allow */
static void G_(Digraph_)(struct G_(Digraph) *const g) {
	if(!g) return;
	G_(VertexListForEach)(&g->vertices, &PG_(v_clear));
	PG_(clear)(g);
}

/** Initialises {g} to an empty {Digraph}.
 @param g: If null, does nothing.
 @order \Theta(1)
 @allow */
static void G_(Digraph)(struct G_(Digraph) *const g) {
	if(!g) return;
	PG_(clear)(g);
}

#ifdef DIGRAPH_VDATA /* <-- vdata */
/** Given {vertex}, gets the associated vertex data; only present if
 {DIGRAPH_VDATA}. Doesn't consider null as a valid function argument. */
static V *G_(DigraphVertexData)(struct G_(Vertex) *const vertex) {
	return &vertex->data;
}
#endif /* vdata --> */

#ifdef DIGRAPH_EDATA /* <-- edata */
/** Given {edge}, gets the associated edge data; only present if
 {DIGRAPH_EDATA}. Doesn't consider null as a valid function argument. */
static E *G_(DigraphEdgeData)(struct G_(Edge) *const edge) {
	return &edge->data;
}
#endif /* edata --> */

/** Undefined behaviour results from adding vertices that have already been
 added. Adds {v} as an isolated vertex in {g}.
 @param g: If null, does nothing but initialise {v}.
 @param v: If null, does nothing, otherwise initialises to contain no edges;
 the vertex data is left alone. */
static void G_(DigraphVertexAdd)(struct G_(Digraph) *const g,
	struct G_(Vertex) *const v) {
	if(!v) return;
	PG_(v_clear)(v);
	if(!g) return;
	if(!G_(VertexListFirst)(&g->vertices)) g->start = v;
	G_(VertexListPush)(&g->vertices, v);
}

/** Undefined behaviour results from adding edges that have already been added.
 Initialises {e} to point to {from} to {to}.
 @param e, from, to: If any are null, does nothing.
 @param e: The edge data is left alone. */
static void G_(DigraphEdgeAdd)(struct G_(Edge) *e,
	struct G_(Vertex) *const from, struct G_(Vertex) *const to) {
	if(!e || !from || !to) return;
	e->to = to;
	G_(EdgeListPush)(&from->out, e);
}

/** Sets the starting vertex. The starting vertex by default is the first
 vertex. */
static void G_(DigraphSetStart)(struct G_(Digraph) *const g,
	struct G_(Vertex) *const start) {
	if(!g) return;
	g->start = start;
}

/** Retrieves the starting vertex or null if {g} is empty or null. */
static struct G_(Vertex) *G_(DigraphGetStart)(struct G_(Digraph) *const g) {
	if(!g) return 0;
	return g->start;
}

/** Appends {g} to {fp} in GraphViz format.
 @param g: If null, does nothing.
 @param fp: File pointer.
 @return Success.
 @throws {fprintf} errors: {IEEE Std 1003.1-2001}.
 @order O(|{vertices}| + |{edges}|)
 @allow */
static int G_(DigraphOut)(const struct G_(Digraph) *const g,
	FILE *const fp) {
	struct G_(Vertex) *v;
	struct G_(Edge) *e;
	char a[12];
	unsigned long v_no, v_to;
	if(!g || !fp) return 0;
	if(fprintf(fp, "digraph " QUOTE(DIGRAPH_NAME) " {\n") < 0) return 0;
	for(v = G_(VertexListFirst)(&g->vertices); v; v = G_(VertexListNext)(v)) {
		v_no = (unsigned long)v;
#ifdef DIGRAPH_VDATA /* <-- vdata */
		PG_(vdata_to_string)(&v->data, &a);
#else /* vdata --><-- !vdata */
		*a = '\0';/*strcpy(a, "");*/
#endif /* !vdata --> */
		if(fprintf(fp, "\tv%lu [label=\"%s\"%s];\n", v_no, a,
			v == g->start ? " peripheries=2" : "") < 0) return 0;
		for(e = G_(EdgeListFirst)(&v->out); e; e = G_(EdgeListNext)(e)) {
			v_to = (unsigned long)e->to;
#ifdef DIGRAPH_EDATA /* <-- edata */
			PG_(edata_to_string)(&e->data, &a);
			if(fprintf(fp, "\tv%lu -> v%lu [label=\"%s\"];\n", v_no, v_to, a)
				< 0) return 0;
#else /* edata --><-- !edata */
			if(fprintf(fp, "\tv%lu -> v%lu;\n", v_no, v_to) < 0) return 0;
#endif /* !edata --> */
		}
	}
	if(fprintf(fp, "\tnode [fillcolor = red];\n"
		"}\n") < 0) return 0;
	return 1;
}

#ifdef DIGRAPH_TEST /* <-- test */
#include "../test/TestDigraph.h" /* Need this file if running tests. */
#endif /* test --> */

/* Prototype. */
static void PG_(unused_coda)(void);
/** This silences unused function warnings from the pre-processor, but allows
 optimisation, (hopefully.)
 \url{ http://stackoverflow.com/questions/43841780/silencing-unused-static-function-warnings-for-a-section-of-code } */
static void PG_(unused)(void) {
	G_(Digraph_)(0);
	G_(Digraph)(0);
#ifdef DIGRAPH_VDATA /* <-- vdata */
	G_(DigraphVertexData)(0);
#endif /* vdata --> */
#ifdef DIGRAPH_EDATA /* <-- edata */
	G_(DigraphEdgeData)(0);
#endif /* edata --> */
	G_(DigraphVertexAdd)(0, 0);
	G_(DigraphEdgeAdd)(0, 0, 0);
	G_(DigraphSetStart)(0, 0);
	G_(DigraphGetStart)(0);
	G_(DigraphOut)(0, 0);
	PG_(unused_coda)();
}
/** {clang}'s pre-processor is not fooled if you have one function. */
static void PG_(unused_coda)(void) { PG_(unused)(); }



/* Un-define all macros. */
/* Undocumented; allows nestled inclusion so long as: {CAT_}, {CAT}, {PCAT},
 {PCAT_} conform, and {G}, {V}, {E}, {T}, {U}, {S}, and {A}, are not used. */
#ifdef DIGRAPH_SUBTYPE /* <-- sub */
#undef DIGRAPH_SUBTYPE
#else /* sub --><-- !sub */
#undef CAT
#undef CAT_
#undef PCAT
#undef PCAT_
#endif /* !sub --> */
#undef G_
#undef PG_
#undef QUOTE
#undef QUOTE_
#ifdef V
#undef V
#endif
#ifdef E
#undef E
#endif
#undef DIGRAPH_NAME
#ifdef DIGRAPH_VDATA
#undef DIGRAPH_VDATA
#endif
#ifdef DIGRAPH_EDATA
#undef DIGRAPH_EDATA
#endif
#ifdef DIGRAPH_VDATA_TO_STRING
#undef DIGRAPH_VDATA_TO_STRING
#endif
#ifdef DIGRAPH_EDATA_TO_STRING
#undef DIGRAPH_EDATA_TO_STRING
#endif
#ifdef DIGRAPH_TEST
#undef DIGRAPH_TEST
#endif
#ifdef DIGRAPH_VDATA_TEST
#undef DIGRAPH_VDATA_TEST
#endif
#ifdef DIGRAPH_EDATA_TEST
#undef DIGRAPH_EDATA_TEST
#endif
#ifdef DIGRAPH_NDEBUG
#undef DIGRAPH_NDEBUG
#undef NDEBUG
#endif
