package model;

import java.util.ArrayList;

public class Job {

	private String name;
	private int startTime;
	private int jobTime;
	private int deadline;
	private String function;
	private String res;
	private ArrayList<Resource> resources = new ArrayList<Resource>();
	
	public Job(String name, int startTime, int jobTime, int deadline, String function, ArrayList<Resource> resources) {
		this.name = name;
		this.startTime = startTime;
		this.jobTime = jobTime;
		this.deadline = deadline;
		this.function = function;
		this.resources = resources;
	}

	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Job) {
			Job j = (Job) obj;
			if (j.getName().equals(name)) return true;
		}
		return false;
	}
	
	public String getRes() {
		return res;
	}

	public void setRes(String res) {
		this.res = res;
	}

	public String getName() {
		return name;
	}

	public int getStartTime() {
		return startTime;
	}

	public int getJobTime() {
		return jobTime;
	}

	public int getDeadline() {
		return deadline;
	}

	public String getFunction() {
		return function;
	}

	public ArrayList<Resource> getResources() {
		return resources;
	}
	
	@Override
	public String toString() {
		return name + ";" + startTime + ";" + jobTime + ";" + deadline + ";" + function + ";" + resources;
	}
	
}
