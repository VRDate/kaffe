/*
 * TemplateNode.java
 * Copyright (C) 2004 The Free Software Foundation
 * 
 * This file is part of GNU JAXP, a library.
 *
 * GNU JAXP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * GNU JAXP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this library, you may extend
 * this exception to your version of the library, but you are not
 * obliged to do so.  If you do not wish to do so, delete this
 * exception statement from your version. 
 */

package gnu.xml.transform;

import java.io.PrintStream;
import java.util.Comparator;
import javax.xml.namespace.QName;
import javax.xml.transform.TransformerException;
import org.w3c.dom.Node;
import gnu.xml.xpath.DocumentOrderComparator;

/**
 * Wrapper for a source node in a template.
 *
 * @author <a href='mailto:dog@gnu.org'>Chris Burdess</a>
 */
abstract class TemplateNode
{

  static final Comparator documentOrderComparator =
    new DocumentOrderComparator();

  final TemplateNode children;
  final TemplateNode next;

  TemplateNode(TemplateNode children, TemplateNode next)
  {
    this.children = children;
    this.next = next;
  }

  final void apply(Stylesheet stylesheet, QName mode,
                   Node context, int pos, int len,
                   Node parent, Node nextSibling)
    throws TransformerException
  {
    if (stylesheet.terminated)
      {
        return;
      }
    if (Thread.currentThread().isInterrupted())
      {
        // Try to head off any infinite loops at the pass
        return;
      }
    if (stylesheet.debug)
      {
        System.err.println("Applying " + toString());
      }
    doApply(stylesheet, mode, context, pos, len, parent, nextSibling);
  }

  abstract void doApply(Stylesheet stylesheet, QName mode,
                        Node context, int pos, int len,
                        Node parent, Node nextSibling)
    throws TransformerException;

  abstract TemplateNode clone(Stylesheet stylesheet);

  /**
   * Debugging
   */
  void list(int depth, PrintStream out, boolean listNext)
  {
    for (int i = 0; i < depth; i++)
      {
        out.print("  ");
      }
    out.println(toString());
    if (children != null)
      {
        children.list(depth + 1, out, true);
      }
    if (listNext && next != null)
      {
        next.list(depth, out, listNext);
      }
  }

}
