<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@page import="utility.Relay"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%
	Relay relay;
	if((relay = (Relay)application.getAttribute("relay")) == null){
		String path = pageContext.getServletContext().getInitParameter("relay_path");
		relay = new Relay(path);
		application.setAttribute("relay", relay);
	}
	int[] relays = new int[4];
	
	//relay 값 가져오기
	for(int i=0;i<4;i++){
		relays[i] = relay.get_relay(i+"");
	}
%>
<c:set var="relays" value="<%=relays%>"/>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1; charset=UTF-8">
<title>멀티탭 제어 페이지</title>
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.1.2/jquery.mobile-1.1.2.min.css"/>
<script src="http://code.jquery.com/jquery-1.7.2.min.js"></script>
<script src="http://code.jquery.com/mobile/1.1.2/jquery.mobile-1.1.2.min.js"></script>
</head>
<body>
<script type="text/javascript">
function relay_chg(index, value){
	location.href = './main.jsp?mode=set&index='+index+'&value='+value;
}
</script>
<div data-role="page">
	<div data-role="header">
		<h1>4-Channer Relay Control</h1>
	</div>
	<div data-role="content">
	<c:forEach begin="0" end="3" varStatus="status">
		<div class="containing-element">
			<label for="flip-min"><c:out value="전원 ${status.count}"/></label>
			<select name="flip-min" id="flip-min" data-role="slider" onchange="relay_chg(${status.index}, this.value);">
				<c:choose>
					<c:when test="${relays[status.index] == 0}">
						<option value="0" selected="selected">OFF</option>
						<option value="1">ON</option>
					</c:when>
					<c:otherwise>
						<option value="0">OFF</option>
						<option value="1" selected="selected">ON</option>
					</c:otherwise>
				</c:choose>
				
			</select>
		</div>
	</c:forEach>
	</div>
	<button id='btnReserve' onclick="location='reserve.jsp'">예약 페이지</button>
</div>
</body>
</html>