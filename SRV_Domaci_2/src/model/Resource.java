package model;

public class Resource {

	private String name;
	private int eat;
	
	public Resource(String name, int eat) {
		this.name = name;
		this.eat = eat;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Resource) {
			Resource r = (Resource) obj;
			if (r.getName().equals(this.name)) return true;
		}
		return false;
	}
	
	public String getName() {
		return name;
	}
	
	public int getEat() {
		return eat;
	}
	
	@Override
	public String toString() {
		return name + "[" + eat + "]";
	}
}
