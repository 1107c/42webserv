export async function checkLoginStatus(curPath)
{
	try {
		const response = await fetch('/check-id', {
			method: 'GET'
		});

		const result = await response.text();
		const data = JSON.parse(result);
		console.log("data: ", data);

		if (data.status == 'login') {
			if (curPath != '/')
			{
				window.location.href = '/';
				return;
			}
			document.getElementById('login-signup').style.display = 'none';
			document.getElementById('user-info').style.display = 'block';
			document.getElementById('user-id').textContent = data.id;
		}
		else if (curPath == '/')
		{
			document.getElementById('login-signup').style.display = 'block';
			document.getElementById('user-info').style.display = 'none';
		}
	} catch (error) {
		console.log("error: ", error);
		window.location.href = '/error';
	}
}
