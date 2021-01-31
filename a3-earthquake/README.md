# CSci-4611 Assignment 3:  Earthquake
Christopher You - CSCI 4611 A3

NOTE:
I had an extremely busy week and had to neglect this homework.  I apologize for not 
providing the A level work that I usually provide.
I have an NSF application that is due on Tuesday that Dan Orban has been helping me extensively on.
Furthermore, I am currently at a conference for ACM SUI 2019 which is in New Orleans and I spent all week preparing for that.
On top of all of that, I had a lot of homework outside of this class.
I was able to complete the B level homework and I attempted the A level homework.
I hope you will take this all into consideration.  Thank you so much in advance.


Things I did:
For Earth.cc
I pushed all the vertices of the rectangles/triangles by going left to right of their points.

I pushed all the indices of the rectangles/triangles that would be added by first adding the first row
Then I added every subsequent row.  The reason why I did this is because the first row starts at 0, and my math was always slightly wrong.

The math for adding the indices is a little complex but essentially I think all I'm doing is taking the current point, adding 11 or adding 12, and pushing all of those
Obviously its 11 and 12 for a 10x10.  It differs depending on the size.

I then add the texture coordinates as needed along the indices.

WIth that I created the LatLongToPlane to use to put the earthquakes on the map.
I also added the function for LatLongToSphere, but I didn't get to fully implement it.
QuakeApp

I take the current time and I find the most recent quake.
I then find the actual Earthquake from that index.
I get its fields and calculate the position in terms of a point3.

The color of the earth is set on a scale of green to red. The redder it is the more powerful it is.
This is calculated through the magnitude of the earth and its "normalized".
I noticed the magnitudes were on a scale of around 6.5 - 7.5, so I simply subtracted 6.5
if it was maxed out at 7.5, then it would be pure read, and if its at 6.5 it would be green.

Then I set a matrix 4 for its position be its point turned into a vector.
I then scaled it to be based on the magnitude of the quake.
Similar idea but this time I divided by 7.5

Before I store this quake into a storage place, I check if it already exists. If it does, then I dont add it again.

Once I start to print the quakes on the screen (the spheres),
I  loop through the quakes, if the current date is 365 days past the date of the quake, i delete  the quake.
If it is less than 365 days, then I shrink the size of the quake slightly and print it out.

I attempted part A with the latitude/longitude, but due to time constraints (its 11:30 pm), I could not continue.
I was busy all day with conference student volunteer events, so this is all I can do.
Thanks for reading!

Chris