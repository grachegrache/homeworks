<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@page import="utility.Relay"%>
<%
	//변수 선언
	Relay relay;
	if((relay = (Relay)application.getAttribute("relay")) == null){
		String path = pageContext.getServletContext().getInitParameter("relay_path");
		relay = new Relay(path);
		application.setAttribute("relay", relay);
	}
	
	String mode = null;
	String index = null;
	String value = null;
	
	//request 처리
	if(request != null){
		mode = request.getParameter("mode");
		index = request.getParameter("index");
		value = request.getParameter("value");
		
		if(mode != null){
			if(mode.equals("get")){
				out.print(relay.get_relay(index));
			}
			if(mode.equals("set")){
				relay.set_relay(index, value);
			}
		}
	}
	
	if(request == null || mode == null || !mode.equals("get")){
		//스위치 제어 페이지로 이동
		response.sendRedirect("./control.jsp");
	}
%>
