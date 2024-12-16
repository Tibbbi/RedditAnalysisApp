import requests
import json
import csv

# Step 1: Set up authentication details
CLIENT_ID = 'TEyAsrbpwkIZXeEaXLnRwg'
CLIENT_SECRET = 'VmvEIneQrbeoJ1enQp--Ky3QbWtrwA'
USERNAME = 'Firm-Ad9920'
PASSWORD = 'LucrareLicenta'
USER_AGENT = 'script:AnalysisApp:v1.0 (by /u/Firm-Ad9920)'

# Step 2: Authenticate and get an access token
def get_access_token():
    auth = requests.auth.HTTPBasicAuth(CLIENT_ID, CLIENT_SECRET)
    data = {
        'grant_type': 'password',
        'username': USERNAME,
        'password': PASSWORD
    }
    headers = {'User-Agent': USER_AGENT}

    response = requests.post('https://www.reddit.com/api/v1/access_token',
                             auth=auth, data=data, headers=headers)

    if response.status_code == 200:
        return response.json()['access_token']
    else:
        print("Failed to get access token:", response.text)
        return None

# Step 3: Save posts to CSV
def save_posts_to_csv(posts, filename="posts.csv"):
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerow(["Title", "Author", "Score", "Comments", "Link"])
        for post in posts:
            writer.writerow([
                post['data']['title'],
                post['data']['author'],
                post['data']['score'],
                post['data']['num_comments'],
                f"https://reddit.com{post['data']['permalink']}"
            ])
    print(f"Posts saved to {filename}")

# Step 4: Fetch subreddit posts
def fetch_hot_posts(subreddit, access_token):
    headers = {
        'Authorization': f'bearer {access_token}',
        'User-Agent': USER_AGENT
    }

    url = f'https://oauth.reddit.com/r/{subreddit}/hot'
    response = requests.get(url, headers=headers)

    if response.status_code == 200:
        posts = response.json()['data']['children']
        save_posts_to_csv(posts)
        for post in posts[:5]:  # Limit to top 5 posts
            print(f"Title: {post['data']['title']}")
            print(f"Author: {post['data']['author']}")
            print(f"Score: {post['data']['score']}")
            print(f"Comments: {post['data']['num_comments']}")
            print(f"Link: https://reddit.com{post['data']['permalink']}\n")
    else:
        print("Failed to fetch posts:", response.text)

# Step 5: Main script to run the application
def main():
    print("Authenticating...")
    access_token = get_access_token()
    
    if access_token:
        subreddit = input("Enter a subreddit to fetch posts from: ")
        print(f"Fetching hot posts from subreddit '{subreddit}'...")
        fetch_hot_posts(subreddit, access_token)

if __name__ == '__main__':
    main()
