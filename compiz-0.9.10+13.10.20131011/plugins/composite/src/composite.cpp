/*
 * Copyright © 2008 Dennis Kasprzyk
 * Copyright © 2007 Novell, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Dennis Kasprzyk not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 * Dennis Kasprzyk makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * DENNIS KASPRZYK DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL DENNIS KASPRZYK BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Authors: Dennis Kasprzyk <onestone@compiz-fusion.org>
 *          David Reveman <davidr@novell.com>
 */

#include "composite/composite.h"

#include "privates.h"

#include "core/abiversion.h"


class CompositePluginVTable :
    public CompPlugin::VTableForScreenAndWindow<CompositeScreen, CompositeWindow, COMPIZ_COMPOSITE_ABI>
{
    public:

	bool init ();
	void fini ();
};

COMPIZ_PLUGIN_20090315 (composite, CompositePluginVTable)

CompOption::Vector &
CompositeScreen::getOptions ()
{
    return priv->getOptions ();
}

bool
CompositeScreen::setOption (const CompString  &name,
			    CompOption::Value &value)
{
    return priv->setOption (name, value);
}

bool
PrivateCompositeScreen::setOption (const CompString  &name,
				   CompOption::Value &value)
{
    unsigned int index;

    bool rv = CompositeOptions::setOption (name, value);

    if (!rv || !CompOption::findOption (getOptions (), name, &index))
	return false;

    switch (index)
    {
	case CompositeOptions::DetectRefreshRate:
	    if (optionGetDetectRefreshRate ())
		detectRefreshRate ();
	    break;

	case CompositeOptions::RefreshRate:
	    if (optionGetDetectRefreshRate ())
		return false;

	    redrawTime = 1000 / optionGetRefreshRate ();
	    optimalRedrawTime = redrawTime;
	    break;

	default:
	    break;
    }

    return rv;
}

bool
CompositePluginVTable::init ()
{
    if (CompPlugin::checkPluginABI ("core", CORE_ABIVERSION))
    {
	CompPrivate p;
	p.uval = COMPIZ_COMPOSITE_ABI;
	screen->storeValue ("composite_ABI", p);
	return true;
    }

    return false;
}

void
CompositePluginVTable::fini ()
{
    screen->eraseValue ("composite_ABI");
}
