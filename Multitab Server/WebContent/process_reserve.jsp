<%@page import="utility.CronTab"%>
<%
	//변수 선언
	String mode,index,hour,minute,relay,onoff;
	CronTab crontab;
	
	if((crontab = (CronTab)application.getAttribute("crontab")) == null){
		String path = pageContext.getServletContext().getInitParameter("relay_path");
		crontab = new CronTab(path);
		application.setAttribute("crontab", crontab);
	}
	
	//request 처리
	if(request != null){
		mode = request.getParameter("mode");
		index = request.getParameter("index");
		hour = request.getParameter("select_hour");
		minute = request.getParameter("select_minute");
		relay = request.getParameter("select_relay");
		onoff = request.getParameter("select_onoff");
		
		if(mode != null){
			if(mode.equals("add")){
				crontab.addReserve(hour, minute, relay, onoff);
				
			}else if(mode.equals("delete")){
				crontab.deleteReserve(Integer.parseInt(index));
			}
		}
	}
	
	//예약 페이지로 이동
	response.sendRedirect("./reserve.jsp");
%>