/* LocaleInformation_dv.java --
   Copyright (C) 2004  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


// This file was automatically generated by gnu.localegen from LDML dv.xml

package gnu.java.locale;

import java.util.ListResourceBundle;

public class LocaleInformation_dv extends ListResourceBundle
{
  private static final class Hashtableterritories extends java.util.Hashtable
  {
    public Hashtableterritories()
      {
        super();
        put("MV", "\u078b\u07a8\u0788\u07ac\u0780\u07a8 \u0783\u07a7\u0787\u07b0\u0796\u07ac");
      }
  }

  private static final Object territories = new Hashtableterritories();

  private static final Object[][] contents =
  {
    { "zeroDigit", "\u0660" },
    { "percentFormat", "#,##,##0%" },
    { "currencyFormat", "\u00a4 #,##,##0.00;-\u00a4 #,##,##0.00" },
    { "territories", territories },
  };

  public Object[][] getContents() { return contents; }
}