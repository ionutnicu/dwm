/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 1;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab			= showtab_auto;        /* Default tab bar show mode */
static const int toptab				= False;               /* False means bottom tab bar */

static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#0034af";

/* solarized colors http://ethanschoonover.com/solarized */
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	{ col_gray3, col_gray1, col_gray2 }, /* SchemeNorm orig */
	{ col_gray4, col_cyan,  col_cyan  }, /* SchemeSel orig */
	{ s_base0, s_base03, s_base2 },      /* SchemeNorm dark */
	{ s_base0, s_base02, s_base2 },      /* SchemeSel dark */
	{ s_base00, s_base3, s_base02 },     /* SchemeNorm light */
	{ s_base00, s_base2, s_base02},      /* SchemeSel light */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
#define TERM "urxvt"
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERM, NULL };
static const char *calcmd[] = { "zenity", "--calendar", NULL };
static const char *xplorercmd[] = { TERM, "-e", "ranger", NULL };
static const char *volupcmd[] = { "pactl", "--", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL};
static const char *voldowncmd[] = { "pactl", "--", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL};
static const char *volmutecmd[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL};
static const char *backlightupcmd[] = { "xbacklight", "-inc", "5", NULL};
static const char *backlightdowncmd[] = { "xbacklight", "-dec", "5", NULL};

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = calcmd } },
	{ MODKEY,                       XK_x,      spawn,          {.v = xplorercmd } },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          {.v = volupcmd } },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          {.v = voldowncmd } },
	{ 0,                XF86XK_AudioMute,      spawn,          {.v = volmutecmd } },
	{ 0,          XF86XK_MonBrightnessUp,      spawn,          {.v = backlightupcmd } },
	{ 0,        XF86XK_MonBrightnessDown,      spawn,          {.v = backlightdowncmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_w,      tabmode,        {-1} },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } }, // Next in stack
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } }, // Prev in stack
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } }, // Next in stack
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } }, // Prev in stack
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } }, // Increase number of items in master
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } }, // Decrease number of items in master
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, // Decrease master area
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} }, // Increase master area
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Down,   movestack,      {.i = +1 } },
	{ MODKEY,                       XK_Up,     movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },          // Make current the master
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, // Focus prev monitor
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } }, // Focus next monitor
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } }, // Move focused window to prev monitor
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, // Move focused window to next monitor
	{ MODKEY|ShiftMask,             XK_t,      schemeToggle,   {0} },
	{ MODKEY|ShiftMask,             XK_z,      schemeCycle,    {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_x,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

