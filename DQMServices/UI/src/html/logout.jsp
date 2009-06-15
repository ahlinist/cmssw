<%@ page import="java.sql.*,java.util.*,java.util.regex.*,cms.dqm.workflow.*,java.security.cert.*" %>

<%

  User user = User.get(request);
  MessageBoardSyn.getInstance().logoutUser(user);
  
  String url = request.getParameter("redirect");
  if (url == null) url = request.getRequestURL().toString();
  url = url.replaceFirst("^https://", "http://");
  response.sendRedirect(url);
  
%>
