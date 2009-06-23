<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%
  User user = User.get(request);
  String mediaurl = WebUtils.getMediaURL();
%>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>

  <meta http-equiv="content-type" content="text/html; charset=utf-8" />
  <title>Request RR access</title>

  <link rel="stylesheet" type="text/css" href="<%=mediaurl%>index.css" />
  <link type="text/css" href="<%=mediaurl%>smoothness/jquery-ui-1.7.1.custom.css" rel="stylesheet" />

</head>
<body>

  <div style="position: relative; width: 80%; left: 100px; top: 50px; background-color: #DDDDDD; padding: 1em">

  <p>Please request access from the responsible person by email. In the email provide the following information:</p>
  <ol>
    <li>describe the reason why you need write access to Run Registry</li>
    <li>list of roles you need (ONLINE, OFFLINE, EXPERT)</li>
    <li>your location at which you will be using Run Registry write access, i.e. FNAL, DESY, personal laptop, etc.</li>
    <li>date if any until which you need write access to RR</li>
    <li>please indicate if this is a user or a host (certificate) based access</li>
    <li>copy and paste the information below
      <br/>
      <br/>
      <div>#==== COPY STARTS HERE</div>
      <div>Username: <%=user.getUsername()%></div>
      <div>CID: <%=user.getCertid()%></div>
      <div>CINFO: <%=user.getCertinfo()%></div>
      <div>#==== COPY ENDS HERE</div>
    </li>
  </ol>
  <div style="text-align: center">
    <a href="main.jsp">Back to table</a>
  </div>
  </div>

</body>
</html>
