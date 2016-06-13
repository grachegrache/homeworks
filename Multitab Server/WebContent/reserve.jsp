<%@page import="java.util.TimeZone"%>
<%@page import="utility.CronTab"%>
<%@page import="java.text.SimpleDateFormat"%>
<%@page import="java.util.Calendar"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1; charset=UTF-8">
<title>예약 페이지</title>
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.1.2/jquery.mobile-1.1.2.min.css"/>
<link rel='stylesheet' type='text/css'href='./css/timepicki.css'/>
<script src="http://code.jquery.com/jquery-1.7.2.min.js"></script>
<script src="http://code.jquery.com/mobile/1.1.2/jquery.mobile-1.1.2.min.js"></script>

<%
	//변수 선언
	CronTab crontab;
	if((crontab = (CronTab)application.getAttribute("crontab")) == null){
		String path = pageContext.getServletContext().getInitParameter("relay_path");
		crontab = new CronTab(path);
		application.setAttribute("crontab", crontab);
	}
	
	//현재 시간 구하기
	//TimeZone tz = TimeZone.getTimeZone("Asia/Seoul");
	Calendar calendar = Calendar.getInstance();
	SimpleDateFormat sdf = new SimpleDateFormat("HH");
	//sdf.setTimeZone(tz);
	String time = sdf.format(calendar.getTime());
	int curHour = Integer.parseInt(time);
	sdf = new SimpleDateFormat("mm");
	//sdf.setTimeZone(tz);
	time = sdf.format(calendar.getTime());
	int curMinute = Integer.parseInt(time);
	
%>

<c:set var="curHour" value="<%=curHour%>"/>
<c:set var="curMinute" value="<%=curMinute%>"/>
<c:set var="crontab" value="<%=crontab.crontab%>"/>

<script type="text/javascript">
$(document).ready(function(){
    $("button#add_button").click(function(){
        $("div#add_form").toggle(100);
    });
    
    $("div#add_form").hide();
});
</script>

</head>
<body>
<div data-role="page">
	<div data-role="header">
		<h1>Reserve Page</h1>
	</div>
	<div data-role="content" align="center">
		<table data-role="table" class="ui-responsive" id="table_reserve">
			<thead align="center">
				<tr>
					<th data-priority="1" width="100">Hour</th>
					<th data-priority="2" width="100">Minute</th>
					<th data-priority="3" width="100">Tab</th>
					<th data-priority="4" width="100">On/Off</th>
					<th data-priority="5" width="100">Delete</th>
				</tr>
			</thead>
			<tbody align="center">
				<c:forEach var="cron" items="${crontab}" varStatus="status">
					<tr>
						<td>${cron.hour}</td>
						<td>${cron.minute}</td>
						<td>${cron.relay}</td>
						<td>${cron.onoff}</td>
						<td>
							<form method="post" action="process_reserve.jsp">
								<input type="submit" value="예약 삭제">
								<input type="hidden" name="mode" value="delete">
								<input type="hidden" name="index" value="${status.index}">
							</form>
						</td>
					</tr>
				</c:forEach>
	      </tbody>
	    </table>
	    <br/><br/><br/>
		<div data-role="footer">
			<button id="add_button">예약 등록 버튼</button>
		</div>
	    <div id="add_form">
			<form method="post" action="process_reserve.jsp">
				<input type="hidden" name="mode" value="add">
				<div data-role="fieldcontain" align="center">
					<label for="select_hour">시:</label>
					<select name="select_hour" id="select_hour">
						<c:forEach var="i" begin="0" end="23">
							<c:choose>
								<c:when test="${curHour == i}">
									<option value="${i}" selected="selected">${i}</option>
								</c:when>
								<c:otherwise>
									<option value="${i}">${i}</option>
								</c:otherwise>
							</c:choose>
						</c:forEach>
					</select>
					<br/><br/>
					<label for="select_minute">분:</label>
					<select name="select_minute" id="select_minute">
						<c:forEach var="i" begin="0" end="59">
							<c:choose>
								<c:when test="${curMinute == i}">
									<option value="${i}" selected="selected">${i}</option>
								</c:when>
								<c:otherwise>
									<option value="${i}">${i}</option>
								</c:otherwise>
							</c:choose>
						</c:forEach>
					</select>
					<br/><br/>
					<label for="select_relay">구 선택:</label>
					<select name="select_relay" id="select_relay">
						<c:forEach var="i" begin="0" end="3">
							<option value="${i}">${i+1}</option>
						</c:forEach>
					</select>
					<br/><br/>
					<label for="select_onoff">전원 온오프:</label>
					<select name="select_onoff" id="select_onoff">
						<option value="1">ON</option>
						<option value="0">OFF</option>
					</select>
					<br/><br/>
					<input type="submit" value="예약 등록">
				</div>
			</form>
		</div>
		<button id='btnBack' onclick="location='control.jsp'">뒤로 가기</button>
	</div>
</div>
</body>
</html>