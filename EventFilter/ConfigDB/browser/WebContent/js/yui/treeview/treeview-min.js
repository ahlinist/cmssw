/*
Copyright (c) 2008, Yahoo! Inc. All rights reserved.
Code licensed under the BSD License:
http://developer.yahoo.net/yui/license.txt
version: 2.5.1
*/
YAHOO.widget.TreeView=function(A){if(A){this.init(A);}};YAHOO.widget.TreeView.prototype={id:null,_el:null,_nodes:null,locked:false,_expandAnim:null,_collapseAnim:null,_animCount:0,maxAnim:2,setExpandAnim:function(A){this._expandAnim=(YAHOO.widget.TVAnim.isValid(A))?A:null;},setCollapseAnim:function(A){this._collapseAnim=(YAHOO.widget.TVAnim.isValid(A))?A:null;},animateExpand:function(C,D){if(this._expandAnim&&this._animCount<this.maxAnim){var A=this;var B=YAHOO.widget.TVAnim.getAnim(this._expandAnim,C,function(){A.expandComplete(D);});if(B){++this._animCount;this.fireEvent("animStart",{"node":D,"type":"expand"});B.animate();}return true;}return false;},animateCollapse:function(C,D){if(this._collapseAnim&&this._animCount<this.maxAnim){var A=this;var B=YAHOO.widget.TVAnim.getAnim(this._collapseAnim,C,function(){A.collapseComplete(D);});if(B){++this._animCount;this.fireEvent("animStart",{"node":D,"type":"collapse"});B.animate();}return true;}return false;},expandComplete:function(A){--this._animCount;this.fireEvent("animComplete",{"node":A,"type":"expand"});},collapseComplete:function(A){--this._animCount;this.fireEvent("animComplete",{"node":A,"type":"collapse"});},init:function(B){this.id=B;if("string"!==typeof B){this._el=B;this.id=this.generateId(B);}this.createEvent("animStart",this);this.createEvent("animComplete",this);this.createEvent("collapse",this);this.createEvent("collapseComplete",this);this.createEvent("expand",this);this.createEvent("expandComplete",this);this._nodes=[];YAHOO.widget.TreeView.trees[this.id]=this;this.root=new YAHOO.widget.RootNode(this);var A=YAHOO.widget.LogWriter;},draw:function(){var A=this.root.getHtml();this.getEl().innerHTML=A;this.firstDraw=false;},getEl:function(){if(!this._el){this._el=document.getElementById(this.id);}return this._el;},regNode:function(A){this._nodes[A.index]=A;},getRoot:function(){return this.root;},setDynamicLoad:function(A,B){this.root.setDynamicLoad(A,B);},expandAll:function(){if(!this.locked){this.root.expandAll();}},collapseAll:function(){if(!this.locked){this.root.collapseAll();}},getNodeByIndex:function(B){var A=this._nodes[B];return(A)?A:null;},getNodeByProperty:function(C,B){for(var A in this._nodes){var D=this._nodes[A];if(D.data&&B==D.data[C]){return D;}}return null;},getNodesByProperty:function(D,C){var A=[];for(var B in this._nodes){var E=this._nodes[B];if(E.data&&C==E.data[D]){A.push(E);}}return(A.length)?A:null;},getNodeByElement:function(C){var D=C,A,B=/ygtv([^\d]*)(.*)/;do{if(D&&D.id){A=D.id.match(B);if(A&&A[2]){return this.getNodeByIndex(A[2]);}}D=D.parentNode;if(!D||!D.tagName){break;}}while(D.id!==this.id&&D.tagName.toLowerCase()!=="body");return null;},removeNode:function(B,A){if(B.isRoot()){return false;}var C=B.parent;if(C.parent){C=C.parent;}this._deleteNode(B);if(A&&C&&C.childrenRendered){C.refresh();}return true;},_removeChildren_animComplete:function(A){this.unsubscribe(this._removeChildren_animComplete);this.removeChildren(A.node);},removeChildren:function(A){if(A.expanded){if(this._collapseAnim){this.subscribe("animComplete",this._removeChildren_animComplete,this,true);YAHOO.widget.Node.prototype.collapse.call(A);return ;}A.collapse();}while(A.children.length){this._deleteNode(A.children[0]);}if(A.isRoot()){YAHOO.widget.Node.prototype.expand.call(A);}A.childrenRendered=false;A.dynamicLoadComplete=false;A.updateIcon();},_deleteNode:function(A){this.removeChildren(A);this.popNode(A);},popNode:function(D){var E=D.parent;var B=[];for(var C=0,A=E.children.length;C<A;++C){if(E.children[C]!=D){B[B.length]=E.children[C];}}E.children=B;E.childrenRendered=false;if(D.previousSibling){D.previousSibling.nextSibling=D.nextSibling;}if(D.nextSibling){D.nextSibling.previousSibling=D.previousSibling;}D.parent=null;D.previousSibling=null;D.nextSibling=null;D.tree=null;delete this._nodes[D.index];},toString:function(){return"TreeView "+this.id;},generateId:function(A){var B=A.id;if(!B){B="yui-tv-auto-id-"+YAHOO.widget.TreeView.counter;++YAHOO.widget.TreeView.counter;}return B;},onExpand:function(A){},onCollapse:function(A){}};YAHOO.augment(YAHOO.widget.TreeView,YAHOO.util.EventProvider);YAHOO.widget.TreeView.nodeCount=0;YAHOO.widget.TreeView.trees=[];YAHOO.widget.TreeView.counter=0;YAHOO.widget.TreeView.getTree=function(B){var A=YAHOO.widget.TreeView.trees[B];return(A)?A:null;};YAHOO.widget.TreeView.getNode=function(B,C){var A=YAHOO.widget.TreeView.getTree(B);return(A)?A.getNodeByIndex(C):null;};YAHOO.widget.TreeView.addHandler=function(B,C,A){if(B.addEventListener){B.addEventListener(C,A,false);}else{if(B.attachEvent){B.attachEvent("on"+C,A);}}};YAHOO.widget.TreeView.removeHandler=function(B,C,A){if(B.removeEventListener){B.removeEventListener(C,A,false);}else{if(B.detachEvent){B.detachEvent("on"+C,A);}}};YAHOO.widget.TreeView.preload=function(F,E){E=E||"ygtv";var C=["tn","tm","tmh","tp","tph","ln","lm","lmh","lp","lph","loading"];var G=[];for(var A=1;A<C.length;A=A+1){G[G.length]='<span class="'+E+C[A]+'">&#160;</span>';}var D=document.createElement("div");var B=D.style;B.className=E+C[0];B.position="absolute";B.height="1px";B.width="1px";B.top="-1000px";B.left="-1000px";D.innerHTML=G.join("");document.body.appendChild(D);YAHOO.widget.TreeView.removeHandler(window,"load",YAHOO.widget.TreeView.preload);};YAHOO.widget.TreeView.addHandler(window,"load",YAHOO.widget.TreeView.preload);YAHOO.widget.Node=function(C,B,A){if(C){this.init(C,B,A);}};YAHOO.widget.Node.prototype={index:0,children:null,tree:null,data:null,parent:null,depth:-1,href:null,target:"_self",expanded:false,multiExpand:true,renderHidden:false,childrenRendered:false,dynamicLoadComplete:false,previousSibling:null,nextSibling:null,_dynLoad:false,dataLoader:null,isLoading:false,hasIcon:true,iconMode:0,nowrap:false,isLeaf:false,_type:"Node",init:function(C,B,A){this.data=C;this.children=[];this.index=YAHOO.widget.TreeView.nodeCount;++YAHOO.widget.TreeView.nodeCount;this.expanded=A;this.createEvent("parentChange",this);if(B){B.appendChild(this);}},applyParent:function(B){if(!B){return false;
}this.tree=B.tree;this.parent=B;this.depth=B.depth+1;if(!this.href){this.href="javascript:"+this.getToggleLink();}this.tree.regNode(this);B.childrenRendered=false;for(var C=0,A=this.children.length;C<A;++C){this.children[C].applyParent(this);}this.fireEvent("parentChange");return true;},appendChild:function(B){if(this.hasChildren()){var A=this.children[this.children.length-1];A.nextSibling=B;B.previousSibling=A;}this.children[this.children.length]=B;B.applyParent(this);if(this.childrenRendered&&this.expanded){this.getChildrenEl().style.display="";}return B;},appendTo:function(A){return A.appendChild(this);},insertBefore:function(A){var C=A.parent;if(C){if(this.tree){this.tree.popNode(this);}var B=A.isChildOf(C);C.children.splice(B,0,this);if(A.previousSibling){A.previousSibling.nextSibling=this;}this.previousSibling=A.previousSibling;this.nextSibling=A;A.previousSibling=this;this.applyParent(C);}return this;},insertAfter:function(A){var C=A.parent;if(C){if(this.tree){this.tree.popNode(this);}var B=A.isChildOf(C);if(!A.nextSibling){this.nextSibling=null;return this.appendTo(C);}C.children.splice(B+1,0,this);A.nextSibling.previousSibling=this;this.previousSibling=A;this.nextSibling=A.nextSibling;A.nextSibling=this;this.applyParent(C);}return this;},isChildOf:function(B){if(B&&B.children){for(var C=0,A=B.children.length;C<A;++C){if(B.children[C]===this){return C;}}}return -1;},getSiblings:function(){return this.parent.children;},showChildren:function(){if(!this.tree.animateExpand(this.getChildrenEl(),this)){if(this.hasChildren()){this.getChildrenEl().style.display="";}}},hideChildren:function(){if(!this.tree.animateCollapse(this.getChildrenEl(),this)){this.getChildrenEl().style.display="none";}},getElId:function(){return"ygtv"+this.index;},getChildrenElId:function(){return"ygtvc"+this.index;},getToggleElId:function(){return"ygtvt"+this.index;},getEl:function(){return document.getElementById(this.getElId());},getChildrenEl:function(){return document.getElementById(this.getChildrenElId());},getToggleEl:function(){return document.getElementById(this.getToggleElId());},getToggleLink:function(){return"YAHOO.widget.TreeView.getNode('"+this.tree.id+"',"+this.index+").toggle()";},collapse:function(){if(!this.expanded){return ;}var A=this.tree.onCollapse(this);if(false===A){return ;}A=this.tree.fireEvent("collapse",this);if(false===A){return ;}if(!this.getEl()){this.expanded=false;}else{this.hideChildren();this.expanded=false;this.updateIcon();}A=this.tree.fireEvent("collapseComplete",this);},expand:function(C){if(this.expanded&&!C){return ;}var A=true;if(!C){A=this.tree.onExpand(this);if(false===A){return ;}A=this.tree.fireEvent("expand",this);}if(false===A){return ;}if(!this.getEl()){this.expanded=true;return ;}if(!this.childrenRendered){this.getChildrenEl().innerHTML=this.renderChildren();}else{}this.expanded=true;this.updateIcon();if(this.isLoading){this.expanded=false;return ;}if(!this.multiExpand){var D=this.getSiblings();for(var B=0;B<D.length;++B){if(D[B]!=this&&D[B].expanded){D[B].collapse();}}}this.showChildren();A=this.tree.fireEvent("expandComplete",this);},updateIcon:function(){if(this.hasIcon){var A=this.getToggleEl();if(A){A.className=this.getStyle();}}},getStyle:function(){if(this.isLoading){return"ygtvloading";}else{var B=(this.nextSibling)?"t":"l";var A="n";if(this.hasChildren(true)||(this.isDynamic()&&!this.getIconMode())){A=(this.expanded)?"m":"p";}return"ygtv"+B+A;}},getHoverStyle:function(){var A=this.getStyle();if(this.hasChildren(true)&&!this.isLoading){A+="h";}return A;},expandAll:function(){for(var A=0;A<this.children.length;++A){var B=this.children[A];if(B.isDynamic()){alert("Not supported (lazy load + expand all)");break;}else{if(!B.multiExpand){alert("Not supported (no multi-expand + expand all)");break;}else{B.expand();B.expandAll();}}}},collapseAll:function(){for(var A=0;A<this.children.length;++A){this.children[A].collapse();this.children[A].collapseAll();}},setDynamicLoad:function(A,B){if(A){this.dataLoader=A;this._dynLoad=true;}else{this.dataLoader=null;this._dynLoad=false;}if(B){this.iconMode=B;}},isRoot:function(){return(this==this.tree.root);},isDynamic:function(){if(this.isLeaf){return false;}else{return(!this.isRoot()&&(this._dynLoad||this.tree.root._dynLoad));}},getIconMode:function(){return(this.iconMode||this.tree.root.iconMode);},hasChildren:function(A){if(this.isLeaf){return false;}else{return(this.children.length>0||(A&&this.isDynamic()&&!this.dynamicLoadComplete));}},toggle:function(){if(!this.tree.locked&&(this.hasChildren(true)||this.isDynamic())){if(this.expanded){this.collapse();}else{this.expand();}}},getHtml:function(){this.childrenRendered=false;var A=[];A[A.length]='<div class="ygtvitem" id="'+this.getElId()+'">';A[A.length]=this.getNodeHtml();A[A.length]=this.getChildrenHtml();A[A.length]="</div>";return A.join("");},getChildrenHtml:function(){var A=[];A[A.length]='<div class="ygtvchildren"';A[A.length]=' id="'+this.getChildrenElId()+'"';if(!this.expanded||!this.hasChildren()){A[A.length]=' style="display:none;"';}A[A.length]=">";if((this.hasChildren(true)&&this.expanded)||(this.renderHidden&&!this.isDynamic())){A[A.length]=this.renderChildren();}A[A.length]="</div>";return A.join("");},renderChildren:function(){var A=this;if(this.isDynamic()&&!this.dynamicLoadComplete){this.isLoading=true;this.tree.locked=true;if(this.dataLoader){setTimeout(function(){A.dataLoader(A,function(){A.loadComplete();});},10);}else{if(this.tree.root.dataLoader){setTimeout(function(){A.tree.root.dataLoader(A,function(){A.loadComplete();});},10);}else{return"Error: data loader not found or not specified.";}}return"";}else{return this.completeRender();}},completeRender:function(){var B=[];for(var A=0;A<this.children.length;++A){B[B.length]=this.children[A].getHtml();}this.childrenRendered=true;return B.join("");},loadComplete:function(){this.getChildrenEl().innerHTML=this.completeRender();this.dynamicLoadComplete=true;this.isLoading=false;this.expand(true);this.tree.locked=false;
},getAncestor:function(B){if(B>=this.depth||B<0){return null;}var A=this.parent;while(A.depth>B){A=A.parent;}return A;},getDepthStyle:function(A){return(this.getAncestor(A).nextSibling)?"ygtvdepthcell":"ygtvblankdepthcell";},getNodeHtml:function(){return"";},refresh:function(){this.getChildrenEl().innerHTML=this.completeRender();if(this.hasIcon){var A=this.getToggleEl();if(A){A.className=this.getStyle();}}},toString:function(){return"Node ("+this.index+")";}};YAHOO.augment(YAHOO.widget.Node,YAHOO.util.EventProvider);YAHOO.widget.TextNode=function(C,B,A){if(C){this.init(C,B,A);this.setUpLabel(C);}};YAHOO.extend(YAHOO.widget.TextNode,YAHOO.widget.Node,{labelStyle:"ygtvlabel",labelElId:null,label:null,textNodeParentChange:function(){if(this.tree&&!this.tree.hasEvent("labelClick")){this.tree.createEvent("labelClick",this.tree);}},setUpLabel:function(A){this.textNodeParentChange();this.subscribe("parentChange",this.textNodeParentChange);if(typeof A=="string"){A={label:A};}this.label=A.label;this.data.label=A.label;if(A.href){this.href=encodeURI(A.href);}if(A.target){this.target=A.target;}if(A.style){this.labelStyle=A.style;}if(A.title){this.title=A.title;}this.labelElId="ygtvlabelel"+this.index;},getLabelEl:function(){return document.getElementById(this.labelElId);},getNodeHtml:function(){var C=[];C[C.length]='<table border="0" cellpadding="0" cellspacing="0">';C[C.length]="<tr>";for(var A=0;A<this.depth;++A){C[C.length]='<td class="'+this.getDepthStyle(A)+'"><div class="ygtvspacer"></div></td>';}var B="YAHOO.widget.TreeView.getNode('"+this.tree.id+"',"+this.index+")";C[C.length]="<td";C[C.length]=' id="'+this.getToggleElId()+'"';C[C.length]=' class="'+this.getStyle()+'"';if(this.hasChildren(true)){C[C.length]=' onmouseover="this.className=';C[C.length]=B+'.getHoverStyle()"';C[C.length]=' onmouseout="this.className=';C[C.length]=B+'.getStyle()"';}C[C.length]=' onclick="javascript:'+this.getToggleLink()+'">';C[C.length]='<div class="ygtvspacer">';C[C.length]="</div>";C[C.length]="</td>";C[C.length]="<td ";C[C.length]=(this.nowrap)?' nowrap="nowrap" ':"";C[C.length]=" >";C[C.length]="<a";C[C.length]=' id="'+this.labelElId+'"';if(this.title){C[C.length]=' title="'+this.title+'"';}C[C.length]=' class="'+this.labelStyle+'"';C[C.length]=' href="'+this.href+'"';C[C.length]=' target="'+this.target+'"';C[C.length]=' onclick="return '+B+".onLabelClick("+B+')"';if(this.hasChildren(true)){C[C.length]=" onmouseover=\"document.getElementById('";C[C.length]=this.getToggleElId()+"').className=";C[C.length]=B+'.getHoverStyle()"';C[C.length]=" onmouseout=\"document.getElementById('";C[C.length]=this.getToggleElId()+"').className=";C[C.length]=B+'.getStyle()"';}C[C.length]=" >";C[C.length]=this.label;C[C.length]="</a>";C[C.length]="</td>";C[C.length]="</tr>";C[C.length]="</table>";return C.join("");},onLabelClick:function(A){return A.tree.fireEvent("labelClick",A);},toString:function(){return"TextNode ("+this.index+") "+this.label;}});YAHOO.widget.RootNode=function(A){this.init(null,null,true);this.tree=A;};YAHOO.extend(YAHOO.widget.RootNode,YAHOO.widget.Node,{getNodeHtml:function(){return"";},toString:function(){return"RootNode";},loadComplete:function(){this.tree.draw();},collapse:function(){},expand:function(){}});YAHOO.widget.HTMLNode=function(D,C,B,A){if(D){this.init(D,C,B);this.initContent(D,A);}};YAHOO.extend(YAHOO.widget.HTMLNode,YAHOO.widget.Node,{contentStyle:"ygtvhtml",contentElId:null,html:null,initContent:function(B,A){this.setHtml(B);this.contentElId="ygtvcontentel"+this.index;this.hasIcon=A;},setHtml:function(B){this.data=B;this.html=(typeof B==="string")?B:B.html;var A=this.getContentEl();if(A){A.innerHTML=this.html;}},getContentEl:function(){return document.getElementById(this.contentElId);},getNodeHtml:function(){var B=[];B[B.length]='<table border="0" cellpadding="0" cellspacing="0">';B[B.length]="<tr>";for(var A=0;A<this.depth;++A){B[B.length]='<td class="'+this.getDepthStyle(A)+'"><div class="ygtvspacer"></div></td>';}if(this.hasIcon){B[B.length]="<td";B[B.length]=' id="'+this.getToggleElId()+'"';B[B.length]=' class="'+this.getStyle()+'"';B[B.length]=' onclick="javascript:'+this.getToggleLink()+'"';if(this.hasChildren(true)){B[B.length]=' onmouseover="this.className=';B[B.length]="YAHOO.widget.TreeView.getNode('";B[B.length]=this.tree.id+"',"+this.index+').getHoverStyle()"';B[B.length]=' onmouseout="this.className=';B[B.length]="YAHOO.widget.TreeView.getNode('";B[B.length]=this.tree.id+"',"+this.index+').getStyle()"';}B[B.length]='><div class="ygtvspacer"></div></td>';}B[B.length]="<td";B[B.length]=' id="'+this.contentElId+'"';B[B.length]=' class="'+this.contentStyle+'"';B[B.length]=(this.nowrap)?' nowrap="nowrap" ':"";B[B.length]=" >";B[B.length]=this.html;B[B.length]="</td>";B[B.length]="</tr>";B[B.length]="</table>";return B.join("");},toString:function(){return"HTMLNode ("+this.index+")";}});YAHOO.widget.MenuNode=function(C,B,A){if(C){this.init(C,B,A);this.setUpLabel(C);}this.multiExpand=false;};YAHOO.extend(YAHOO.widget.MenuNode,YAHOO.widget.TextNode,{toString:function(){return"MenuNode ("+this.index+") "+this.label;}});YAHOO.widget.TVAnim=function(){return{FADE_IN:"TVFadeIn",FADE_OUT:"TVFadeOut",getAnim:function(B,A,C){if(YAHOO.widget[B]){return new YAHOO.widget[B](A,C);}else{return null;}},isValid:function(A){return(YAHOO.widget[A]);}};}();YAHOO.widget.TVFadeIn=function(A,B){this.el=A;this.callback=B;};YAHOO.widget.TVFadeIn.prototype={animate:function(){var D=this;var C=this.el.style;C.opacity=0.1;C.filter="alpha(opacity=10)";C.display="";var B=0.4;var A=new YAHOO.util.Anim(this.el,{opacity:{from:0.1,to:1,unit:""}},B);A.onComplete.subscribe(function(){D.onComplete();});A.animate();},onComplete:function(){this.callback();},toString:function(){return"TVFadeIn";}};YAHOO.widget.TVFadeOut=function(A,B){this.el=A;this.callback=B;};YAHOO.widget.TVFadeOut.prototype={animate:function(){var C=this;var B=0.4;var A=new YAHOO.util.Anim(this.el,{opacity:{from:1,to:0.1,unit:""}},B);A.onComplete.subscribe(function(){C.onComplete();
});A.animate();},onComplete:function(){var A=this.el.style;A.display="none";A.filter="alpha(opacity=100)";this.callback();},toString:function(){return"TVFadeOut";}};YAHOO.register("treeview",YAHOO.widget.TreeView,{version:"2.5.1",build:"984"});