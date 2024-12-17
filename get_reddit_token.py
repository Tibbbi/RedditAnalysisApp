import requests
import sys

# Reddit API credentials
CLIENT_ID = 'TEyAsrbpwkIZXeEaXLnRwg'
CLIENT_SECRET = 'VmvEIneQrbeoJ1enQp--Ky3QbWtrwA'
USERNAME = 'Firm-Ad9920'
PASSWORD = 'LucrareLicenta'
USER_AGENT = 'script:AnalysisApp:v1.0 (by /u/Firm-Ad9920)'

def get_bearer_token():
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
        token = response.json().get('access_token')
        if token:
            print(token)
            return
    print(f"Error: {response.status_code} - {response.text}", file=sys.stderr)
    sys.exit(1)

if __name__ == "__main__":
    get_bearer_token()
